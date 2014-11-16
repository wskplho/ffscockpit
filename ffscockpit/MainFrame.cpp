/*!
* \file MainFrame.cpp
* \brief Main Frame Class
* \author Gianni.P
* \version 0.1
* \date 15/11/2014
*
* Logiciel de couplage de simulateur de vol avec un cockpit basé sur arduino
*
*/

#include "MainFrame.h"
#include "Global.h"
#include "AboutBox.h"
#include "wx/numdlg.h"

#include "arddeco.xpm"
#include "ardconn.xpm"
#include "fsxdeco.xpm"
#include "fsxconn.xpm"
#include "outils.xpm"
#include "fileopen.xpm"
#include "filesave.xpm"
#include "filesaveas.xpm"

wxBEGIN_EVENT_TABLE(CMainFrame, wxFrame)

	EVT_MENU (MF_MENU_NEW, CMainFrame::OnNew)
	EVT_MENU (MF_MENU_OPEN, CMainFrame::OnLoad)
	EVT_MENU (MF_MENU_SAVE, CMainFrame::OnSave)
	EVT_MENU (MF_MENU_SAVE_AS,	CMainFrame::OnSaveAs)
	EVT_MENU (MF_MENU_EXIT, CMainFrame::OnExit)
	EVT_MENU (MF_MENU_ABOUT, CMainFrame::OnAbout)
	EVT_MENU (MF_MENU_VIEW_LOG,	CMainFrame::OnView)
	EVT_MENU (MF_MENU_VIEW_IN, CMainFrame::OnView)
	EVT_MENU (MF_MENU_VIEW_OUT, CMainFrame::OnView)
	EVT_MENU (MF_MENU_FSX_CON,	CMainFrame::OnSimCon)
	EVT_MENU (MF_MENU_ARD_CON,	CMainFrame::OnArdCon)
	EVT_MENU (MF_MENU_ARD_SETUP, CMainFrame::OnArdSetup)
	EVT_IDLE (CMainFrame::OnIdle)
	EVT_CLOSE (CMainFrame::OnCloseWindow)

wxEND_EVENT_TABLE()

//------------------------------------------------------------------------------
// Constructeur

CMainFrame::CMainFrame
	(
	) : wxFrame
	(
		(wxWindow*)NULL,
		wxID_ANY,
		AppName,
		wxDefaultPosition,
		wxDefaultSize,
		wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxRESIZE_BORDER | wxSYSTEM_MENU |
		wxCAPTION | wxCLOSE_BOX
	)
{
	pConfig = wxConfigBase::Get();
	m_mgr.SetManagedWindow (this);
	AFSXCon=false;
	AArdCon = false;

	int	x = pConfig->Read (_T ("/Fenetre/prinx"), 50),
		y = pConfig->Read (_T ("/Fenetre/priny"), 50),
		w = pConfig->Read (_T ("/Fenetre/prinw"), 665),
		h = pConfig->Read (_T ("/Fenetre/prinh"), 660);
	if((x<1) || (y<1))
	{
		x = 50;
		y = 50;
	}
	ProjectFileName = pConfig->Read (_T ("/Project/LastFileName"), _T ("default.ffsc"));
	SetSize (wxSize (w, h));
	SetPosition (wxPoint (x, y));
	SetSizeHints (665, 660);

	wxMenu* MenuFile = new wxMenu;
	MenuFile->Append(MF_MENU_NEW, _("&New"),_("New project"));
	MenuFile->Append(MF_MENU_OPEN, _("&Open"),_("Open a project file"));
	MenuFile->Append(MF_MENU_SAVE, _("&Save"),_("Save the project to file"));
	MenuFile->Append(MF_MENU_SAVE_AS, _("&Save As"),_("Save the project to file"));
    MenuFile->Append(MF_MENU_EXIT, _("E&xit"),_("Exiting BackupAw"));

	wxMenu* MenuView = new wxMenu;
	MenuView->Append (MF_MENU_VIEW_LOG, _ ("Event Log"), _ ("Event log viewer"));
	MenuView->Append (MF_MENU_VIEW_IN, _ ("Serial In"), _ ("Serial In data viewer"));
	MenuView->Append (MF_MENU_VIEW_OUT, _ ("Serial Out"), _ ("Serial In data viewer"));

	wxMenu* MenuSimCon = new wxMenu;
	MenuSimCon->Append(MF_MENU_FSX_CON, _("Connect"), _("Connect to flight simulator"));

    wxMenu* MenuArduino = new wxMenu;
	MenuArduino->Append(MF_MENU_ARD_CON, _("Connexion"), _("Connect to Arduino network"));
	MenuArduino->Append(MF_MENU_ARD_SETUP, _("COM Setup"), _("Configure Communication to Arduino network"));

    wxMenu* MenuHelp = new wxMenu;
	MenuHelp->Append(MF_MENU_ABOUT, _("A&bout"), _("About the FFSCocpkit"));

	wxMenuBar* Menu = new wxMenuBar;
	Menu->Append(MenuFile, _("&Project"));
	Menu->Append(MenuView, _("&View"));
	Menu->Append(MenuSimCon, _("&SimConnect"));
	Menu->Append(MenuArduino, _("&Arduino"));
	Menu->Append(MenuHelp, _("&Help"));

    SetMenuBar(Menu);

	ToolBar = CreateToolBar(wxTB_HORIZONTAL/*|wxTB_HORZ_LAYOUT*/);
	ToolBar->SetToolBitmapSize (wxSize (24,24));
	ToolBar->AddTool ( MF_MENU_OPEN, _("Open"), wxBitmap(fileopen_xpm), _("Open Project"));
	ToolBar->AddTool ( MF_MENU_SAVE, _("Save"), wxBitmap(filesave_xpm), _("Save Project"));
	ToolBar->AddTool ( MF_MENU_SAVE_AS, _("Save As"), wxBitmap(filesaveas_xpm), _("Save Project"));
	ToolBar->AddTool ( MF_MENU_FSX_CON, _("deco"), wxBitmap(fsxdeco_xpm), _("FSX Connect"));
	ToolBar->AddTool (MF_MENU_ARD_CON, _ ("deco"), wxBitmap (arddeco_xpm), _ ("Arduino Connect"));
	ToolBar->AddTool ( MF_MENU_ARD_SETUP, _("COM Setup"), wxBitmap(outils_xpm), _("Configure Communication to Arduino network"));
	ToolBar->Realize ();
	//SetIcon (wxIcon(backupvp_xpm));

    CreateStatusBar(3);
	LogZone = new wxTextCtrl
		(
			this,
			wxID_ANY,
			wxEmptyString,
			wxDefaultPosition,
			wxDefaultSize,
			wxTE_MULTILINE |
			wxTE_READONLY |
			wxTE_RICH
		);
	InData = new wxTextCtrl
		(
		this,
		wxID_ANY,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxTE_MULTILINE |
		wxTE_READONLY |
		wxTE_RICH
		);
	InData->SetDefaultStyle (wxTextAttr (wxColour(_T("#008000"))));
	OutData = new wxTextCtrl
		(
		this,
		wxID_ANY,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxTE_MULTILINE |
		wxTE_READONLY |
		wxTE_RICH
		);
	OutData->SetDefaultStyle (wxTextAttr (*wxRED));
	wxLog::SetActiveTarget(new wxLogTextCtrl(LogZone));

	m_mgr.AddPane (LogZone,wxAuiPaneInfo().Name(_T("LogZone")).Top().Caption(_T ("LogZone")));
	m_mgr.AddPane (InData, wxAuiPaneInfo ().Name (_T("InData")).Bottom().Caption(_T("InData")));
	m_mgr.AddPane (OutData, wxAuiPaneInfo ().Name (_T("OutData")).Bottom().Caption (_T ("OutData")));
	m_mgr.Update ();
	if (m_mgr.LoadPerspective (pConfig->Read (_T ("/Fenetre/perspective"), _T ("")))) m_mgr.Update ();
	wxLogMessage (AppName + _(" Started."));

	SimCon = CSimCon::Instance();
	SerialCtrl = CSerialCtrl::Instance();
	SerialCtrl->SetInView (InData);
	SerialCtrl->SetOutView (OutData);
	TUpdate=new wxTimer (this, MF_UPDATE);
	TUpdate->Start (100, false);
}

//------------------------------------------------------------------------------
// Click sur le menu Exit

void CMainFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
    Close();
}

//------------------------------------------------------------------------------
// Fermeture de la fenetre

void CMainFrame::OnCloseWindow(wxCloseEvent& WXUNUSED(event))
{
	SimCon->Connect(false);
	TUpdate->Stop();
	int x, y, w, h;
	if (IsMaximized())
	{
		Maximize(false);
	}
	if (IsIconized())
	{
	    Iconize (false);
	}
	GetSize(&w, &h);
	GetPosition(&x, &y);
	pConfig->Write(_T("/Fenetre/prinx"), x);
	pConfig->Write(_T("/Fenetre/priny"), y);
	pConfig->Write(_T("/Fenetre/prinw"), w);
	pConfig->Write(_T("/Fenetre/prinh"), h);
	pConfig->Write (_T ("/Fenetre/perspective"), m_mgr.SavePerspective ());
	pConfig->Flush();
	m_mgr.UnInit ();
	Destroy();
}

//------------------------------------------------------------------------------

void CMainFrame::OnView (wxCommandEvent& event)
{
	switch(event.GetId ())
	{
	case MF_MENU_VIEW_LOG:
		m_mgr.GetPane (_T ("LogZone")).Show();
		break;
	case MF_MENU_VIEW_IN:
		m_mgr.GetPane (_T ("InData")).Show();
		break;
	case MF_MENU_VIEW_OUT:
		m_mgr.GetPane (_T ("OutData")).Show();
		break;
	default:
		return;
	}
	m_mgr.Update ();
}
//------------------------------------------------------------------------------

void CMainFrame::OnSimCon (wxCommandEvent& WXUNUSED(event))
{
	SimCon->Connect(!SimCon->IsConnected());
}

//------------------------------------------------------------------------------

void CMainFrame::OnArdCon (wxCommandEvent& WXUNUSED(event))
{
	SerialCtrl->Connect(!SerialCtrl->IsConnected());
}

//------------------------------------------------------------------------------

void CMainFrame::OnArdSetup (wxCommandEvent& WXUNUSED(event))
{

}



//------------------------------------------------------------------------------
// Click sur About

void CMainFrame::OnAbout (wxCommandEvent& WXUNUSED(event))
{
    AboutBox aboutDialog	(	this,
								_("About..."),
								wxPoint(100,100),
								wxSize(400,300),
								wxDEFAULT_DIALOG_STYLE
							);
	aboutDialog.ShowModal();
}

//------------------------------------------------------------------------------
// Mise a jour périodique des éléments dynamiques de la fenêtre principale

void CMainFrame::OnIdle (wxIdleEvent& event)
{
	wxString s;
	wxMenuBar* Menu = GetMenuBar ();
	wxMenuItem* ItemSimCon = Menu->FindItem (MF_MENU_FSX_CON);
	wxMenuItem* ItemArdCon = Menu->FindItem(MF_MENU_ARD_CON);
	// Mise à jour des icones de connection
	if(SimCon->IsConnected () && (!AFSXCon))
	{
		ItemSimCon->SetItemLabel(_("Disconnect"));
		ItemSimCon->SetHelp(_("Flight Simulator Disconnection"));
		ToolBar->DeleteTool (MF_MENU_FSX_CON);
		ToolBar->InsertTool (3, MF_MENU_FSX_CON, _ ("FSX Disconnect"), wxBitmap (fsxconn_xpm), wxNullBitmap, wxITEM_NORMAL, _ ("Flight Simulator Disconnection"));
		AFSXCon = true;
		ToolBar->Realize();
	}
	else if((!SimCon->IsConnected ()) && (AFSXCon))
	{
		ItemSimCon->SetItemLabel(_("Connect"));
		ItemSimCon->SetHelp(_("Flight Simulator Connection"));
		ToolBar->DeleteTool (MF_MENU_FSX_CON);
		ToolBar->InsertTool (3, MF_MENU_FSX_CON, _ ("FSX Connect"), wxBitmap (fsxdeco_xpm), wxNullBitmap, wxITEM_NORMAL, _ ("Flight Simulator Disconnection"));
		AFSXCon = false;
		ToolBar->Realize();
	}
	if (SerialCtrl->IsConnected() && (!AArdCon))
	{
		ItemArdCon->SetItemLabel(_("Disconnect"));
		ItemArdCon->SetHelp(_("Arduino Disconnection"));
		ToolBar->DeleteTool (MF_MENU_ARD_CON);
		ToolBar->InsertTool(4, MF_MENU_ARD_CON, _("Disconnect"), wxBitmap(ardconn_xpm), wxNullBitmap, wxITEM_NORMAL, _("Arduino Disconnection"));
		AArdCon = true;
		ToolBar->Realize();
	}
	else if ((!SerialCtrl->IsConnected()) && (AArdCon))
	{
		ItemArdCon->SetItemLabel(_("Connect"));
		ItemArdCon->SetHelp(_("Arduino Connection"));
		ToolBar->DeleteTool (MF_MENU_ARD_CON);
		ToolBar->InsertTool (4, MF_MENU_ARD_CON, _ ("Connect"), wxBitmap (arddeco_xpm), wxNullBitmap, wxITEM_NORMAL, _ ("Arduino Disconnection"));
		AArdCon = false;
		ToolBar->Realize();
	}
	event.Skip();
}

//------------------------------------------------------------------------------

void CMainFrame::OnNew (wxCommandEvent& WXUNUSED(event))
{

}

//------------------------------------------------------------------------------

void CMainFrame::OnLoad (wxCommandEvent& WXUNUSED(event))
{
	wxFileDialog ProjectFile
		(
		this,
		_("Open Project"),
		ProjectFileName.GetPath(),
		_T("*.ffsc"),
		_("FFSCocpkit Project (*.ffsc)|*.ffsc|All Files (*.*)|*.*"),
		wxFD_OPEN | wxFD_CHANGE_DIR,
		wxDefaultPosition
		);
	if (ProjectFile.ShowModal() == wxID_OK)
	{
		ProjectFileName = ProjectFile.GetPath();
	}
}

//------------------------------------------------------------------------------

void CMainFrame::OnSave (wxCommandEvent& event)
{

}

//------------------------------------------------------------------------------

void CMainFrame::OnSaveAs (wxCommandEvent& WXUNUSED(event))
{
	wxString SaveFileName;
	wxFileDialog ProjectFile
	(
		this,
		_("Save Project"),
		ProjectFileName.GetPath(),
		_T("*.ffsc"),
		_("FFSCocpkit Project (*.ffsc)|*.ffsc|All Files (*.*)|*.*"),
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_CHANGE_DIR,
		wxDefaultPosition
	);
	if (ProjectFile.ShowModal() == wxID_OK)
	{
		ProjectFileName = ProjectFile.GetPath();
		pConfig->Write(_T("/Project/LastFileName"), ProjectFileName.GetFullPath());
		pConfig->Flush();
	}
}

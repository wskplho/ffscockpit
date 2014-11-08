// *****************************************************************************
// *                                                                           *
// *                      FFSCockpit : MainFrame.cpp                           *
// *                           Main Frame class                                *
// *                                                                           *
// *****************************************************************************
// *                                                                           *
// *   CopyRight 2014 Neophile                                                 *
// *   Creation          : 18/10/2014                                          *
// *   Revision          : A                                                   *
// *                                                                           *
// *****************************************************************************

#include "MainFrame.h"
#include "Global.h"
#include "AboutBox.h"
#include "wx/numdlg.h"

#include "deco.xpm"
#include "connect.xpm"
#include "outils.xpm"
#include "fileopen.xpm"
#include "filesave.xpm"
#include "filesaveas.xpm"

wxBEGIN_EVENT_TABLE(CMainFrame, wxFrame)

	EVT_MENU	(MF_MENU_NEW,		CMainFrame::OnNew)
	EVT_MENU	(MF_MENU_OPEN,		CMainFrame::OnLoad)
	EVT_MENU	(MF_MENU_SAVE,		CMainFrame::OnSave)
	EVT_MENU	(MF_MENU_SAVE_AS,	CMainFrame::OnSaveAs)
	EVT_MENU	(MF_MENU_EXIT,		CMainFrame::OnExit)
	EVT_MENU	(MF_MENU_ABOUT,		CMainFrame::OnAbout)
	EVT_MENU	(MF_MENU_SIMCON,	CMainFrame::OnSimCon)
	EVT_MENU	(MF_MENU_ARD_CON,	CMainFrame::OnArdCon)
	EVT_MENU	(MF_MENU_ARD_SETUP, CMainFrame::OnArdSetup)
	EVT_IDLE	(					CMainFrame::OnIdle)
	EVT_CLOSE	(					CMainFrame::OnCloseWindow)

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

	ASimCon=false;
	AArdCon = false;

	wxMenu* MenuFile = new wxMenu;
	MenuFile->Append(MF_MENU_NEW, _("&New"),_("New project"));
	MenuFile->Append(MF_MENU_OPEN, _("&Open"),_("Open a project file"));
	MenuFile->Append(MF_MENU_SAVE, _("&Save"),_("Save the project to file"));
	MenuFile->Append(MF_MENU_SAVE_AS, _("&Save As"),_("Save the project to file"));
	MenuFile->AppendSeparator ();
    MenuFile->Append(MF_MENU_EXIT, _("E&xit"),_("Exiting BackupAw"));

	wxMenu* MenuSimCon = new wxMenu;
	MenuSimCon->Append(MF_MENU_SIMCON, _("Connect"), _("Connect to flight simulator"));

    wxMenu* MenuArduino = new wxMenu;
	MenuArduino->Append(MF_MENU_ARD_CON, _("Connexion"), _("Connect to Arduino network"));
	MenuArduino->Append(MF_MENU_ARD_SETUP, _("COM Setup"), _("Configure Communication to Arduino network"));

    wxMenu* MenuHelp = new wxMenu;
	MenuHelp->Append(MF_MENU_ABOUT, _("A&bout"), _("About the FFSCocpkit"));

	wxMenuBar* Menu = new wxMenuBar;
	Menu->Append(MenuFile, _("&Project"));
	Menu->Append(MenuSimCon, _("&SimConnect"));
	Menu->Append(MenuArduino, _("&Arduino"));
	Menu->Append(MenuHelp, _("&Help"));

    SetMenuBar(Menu);

	ToolBar = CreateToolBar(wxTB_HORIZONTAL/*|wxTB_HORZ_LAYOUT*/);
	ToolBar->SetToolBitmapSize (wxSize (24,24));
	ToolBar->AddTool ( MF_MENU_OPEN, _("Open"), wxBitmap(fileopen_xpm), _("Open Project"));
	ToolBar->AddTool ( MF_MENU_SAVE, _("Save"), wxBitmap(filesave_xpm), _("Save Project"));
	ToolBar->AddTool ( MF_MENU_SAVE_AS, _("Save As"), wxBitmap(filesaveas_xpm), _("Save Project"));
	ToolBar->AddTool ( MF_MENU_SIMCON, _("deco"), wxBitmap(deco_xpm), _("Connect"));
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
	wxLog::SetActiveTarget(new wxLogTextCtrl(LogZone));

	SizerPrin = new wxBoxSizer(wxVERTICAL);
	SizerPrin->Add(LogZone, 1, wxEXPAND);
	SetSizer(SizerPrin);
	wxLogMessage (AppName + _(" Started."));

	int	x = pConfig->Read(_T("/Fenetre/prinx"), 50),
		y = pConfig->Read(_T("/Fenetre/priny"), 50),
		w = pConfig->Read(_T("/Fenetre/prinw"), 665),
		h = pConfig->Read(_T("/Fenetre/prinh"), 660);
    if ((x<1) || (y<1))
    {
        x=50;
        y=50;
    }
	ProjectFileName = pConfig->Read(_T("/Project/LastFileName"), _T("default.ffsc"));
	SetSize(wxSize(w,h));
	SetPosition (wxPoint(x,y));
	SetSizeHints (665,660);
	SimCon = new CSimCon();
	SerialCtrl = CSerialCtrl::GetInstance();
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
	pConfig->Flush();
	Destroy();
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
	wxMenuItem* ItemSimCon = Menu->FindItem (MF_MENU_SIMCON);
	wxMenuItem* ItemArdCon = Menu->FindItem(MF_MENU_ARD_CON);
	// Mise à jour des icones de connection
	if (SimCon->IsConnected() && (!ASimCon))
	{
		ItemSimCon->SetItemLabel(_("Disconnect"));
		ItemSimCon->SetHelp(_("Flight Simulator Disconnection"));
		ToolBar->DeleteTool (MF_MENU_SIMCON);
		ToolBar->InsertTool(3, MF_MENU_SIMCON, _("Disconnect"), wxBitmap(connect_xpm), wxNullBitmap, wxITEM_NORMAL, _("Flight Simulator Disconnection"));
		ASimCon = true;
		ToolBar->Realize();
	}
	else if ((!SimCon->IsConnected()) && (ASimCon))
	{
		ItemSimCon->SetItemLabel(_("Connect"));
		ItemSimCon->SetHelp(_("Flight Simulator Connection"));
		ToolBar->DeleteTool(MF_MENU_SIMCON);
		ToolBar->InsertTool(3, MF_MENU_SIMCON, _("Connect"), wxBitmap(deco_xpm), wxNullBitmap, wxITEM_NORMAL, _("Flight Simulator Disconnection"));
		ASimCon = false;
		ToolBar->Realize();
	}
	if (SerialCtrl->IsConnected() && (!AArdCon))
	{
		ItemArdCon->SetItemLabel(_("Disconnect"));
		ItemArdCon->SetHelp(_("Arduino Disconnection"));
		//ToolBar->DeleteTool(MF_MENU_SIMCON);
		//ToolBar->InsertTool(3, MF_MENU_SIMCON, _("Disconnect"), wxBitmap(connect_xpm), wxNullBitmap, wxITEM_NORMAL, _("Flight Simulator Disconnection"));
		AArdCon = true;
		//ToolBar->Realize();
	}
	else if ((!SerialCtrl->IsConnected()) && (AArdCon))
	{
		ItemArdCon->SetItemLabel(_("Connect"));
		ItemArdCon->SetHelp(_("Arduino Simulator Connection"));
		//ToolBar->DeleteTool(MF_MENU_SIMCON);
		//ToolBar->InsertTool(3, MF_MENU_SIMCON, _("Connect"), wxBitmap(deco_xpm), wxNullBitmap, wxITEM_NORMAL, _("Flight Simulator Disconnection"));
		AArdCon = false;
		//ToolBar->Realize();
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

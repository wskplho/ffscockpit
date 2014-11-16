#pragma once

/*!
* \file MainFrame.h
* \brief Main Frame Class
* \author Gianni.P
* \version 0.1
* \date 15/11/2014
*
* Logiciel de couplage de simulateur de vol avec un cockpit basé sur arduino
*
*/

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wx/config.h"
#include "wx/toolbar.h"
#include "wx/filename.h"
#include "wx/aui/aui.h"

#include "SimCon.h"
#include "SerialCtrl.h"

enum MF_COMMANDS
{
	MF_MENU_NEW = wxID_HIGHEST,
	MF_MENU_OPEN,
	MF_MENU_SAVE,
	MF_MENU_SAVE_AS,
	MF_MENU_EXIT,
	MF_MENU_VIEW_LOG,
	MF_MENU_VIEW_IN,
	MF_MENU_VIEW_OUT,
	MF_MENU_FSX_CON,
	MF_MENU_ARD_CON,
	MF_MENU_ARD_SETUP,
	MF_MENU_ABOUT,
	MF_UPDATE
};

class CMainFrame : public wxFrame
{
	public:
								CMainFrame ();

	private:
		wxAuiManager m_mgr;
		wxTextCtrl*				LogZone;
		wxTextCtrl*				InData;
		wxTextCtrl*				OutData;
		wxConfigBase* 			pConfig;
		wxToolBar*				ToolBar;
        wxTimer*                TUpdate;
		bool					AFSXCon;
		bool					AArdCon;
		wxFileName				ProjectFileName;
		CSimCon*				SimCon;
		CSerialCtrl*			SerialCtrl;

	protected:

		void				OnNew (wxCommandEvent& event);
		void				OnLoad (wxCommandEvent& event);
		void				OnSave (wxCommandEvent& event);
		void				OnSaveAs (wxCommandEvent& event);
		void				OnExit(wxCommandEvent& event);
		void				OnView (wxCommandEvent& event);
		void				OnSimCon(wxCommandEvent& event);
		void				OnArdCon(wxCommandEvent& event);
		void				OnArdSetup(wxCommandEvent& event);
		void				OnAbout (wxCommandEvent& event);
		void                OnIdle (wxIdleEvent& event);
		void				OnCloseWindow (wxCloseEvent& event);
		wxDECLARE_EVENT_TABLE();
};



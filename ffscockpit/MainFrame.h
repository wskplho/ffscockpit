// *****************************************************************************
// *                                                                           *
// *                       FFSCOCPKIT : MainFrame.h                            *
// *                           Main Frame class                                *
// *                                                                           *
// *****************************************************************************
// *                                                                           *
// *   Copyright 2014 Neophile                                                 *
// *   Creation          : 18/10/2014                                          *
// *   Last Modification : 20/04/2014                                          *
// *   Revision          : A                                                   *
// *                                                                           *
// *****************************************************************************

#ifndef MAINFRAME_H
#define MAINFRAME_H

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

#include "SimCon.h"
#include "SerialCtrl.h"

enum MF_COMMANDS
{
	MF_MENU_NEW = wxID_HIGHEST,
	MF_MENU_OPEN,
	MF_MENU_SAVE,
	MF_MENU_SAVE_AS,
	MF_MENU_EXIT,
	MF_MENU_SIMCON,
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
		wxTextCtrl*				LogZone;
		wxBoxSizer* 			SizerPrin;
		wxConfigBase* 			pConfig;
		wxToolBar*				ToolBar;
        wxTimer*                TUpdate;
		bool					ASimCon;
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
		void				OnSimCon(wxCommandEvent& event);
		void				OnArdCon(wxCommandEvent& event);
		void				OnArdSetup(wxCommandEvent& event);
		void				OnAbout (wxCommandEvent& event);
		void                OnIdle (wxIdleEvent& event);
		void				OnCloseWindow (wxCloseEvent& event);
		wxDECLARE_EVENT_TABLE();
};

#endif


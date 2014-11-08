// *****************************************************************************
// *                                                                           *
// *                           FFSCockpit : About.h                            *
// *                              About Dialog                                 *
// *                                                                           *
// *****************************************************************************
// *                                                                           *
// *   CopyRight 2014      Neophile                                            *
// *   Creation          : 18/10/2014                                          *
// *   Revision          : A                                                   *
// *                                                                           *
// *****************************************************************************

#ifndef ABOUTBOX_H
#define ABOUTBOX_H

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
	#include <wx/wx.h>
#endif

#include "wx/config.h"

enum
{
	ID_SB_WXWINDOWS = wxID_HIGHEST,
	ID_BTHOME
};


class AboutBox: public wxDialog
{
	public:
						AboutBox
						(
							wxWindow *parent,
							const wxString &title,
							const wxPoint &pos = wxDefaultPosition,
							const wxSize &size = wxDefaultSize,
							long style = wxDEFAULT_DIALOG_STYLE
						);
						~AboutBox ();
	private:
		wxConfigBase*	pConfig;
		//wxStaticText	*StaticText;
	protected:
		void			On_lien_site (wxCommandEvent & event);
		void			On_lien_wxwidget (wxCommandEvent & event);
		wxDECLARE_EVENT_TABLE();
};

#endif


// *****************************************************************************
// *                                                                           *
// *                             APPLICATION CLASS                             *
// *                                                                           *
// *****************************************************************************
// *                                                                           *
// *   CopyRight 2014      Neophile                                            *
// *   Creation          : 18/10/2014                                          *
// *   Revision          : A                                                   *
// *                                                                           *
// *****************************************************************************

#ifndef MAINAPP_H
#define MAINAPP_H

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/intl.h"
#include "wx/fileconf.h"

#define NB_MAX_LNG 2

// language data
static const wxLanguage langIds[] =
{
	wxLANGUAGE_ENGLISH,
	wxLANGUAGE_FRENCH
};

const wxString langNames[] =
{
	_T("English"),
	_T("French")
};

wxCOMPILE_TIME_ASSERT(WXSIZEOF(langNames) == WXSIZEOF(langIds),
	LangArraysMismatch);

class CMainApp : public wxApp
{
private:
	wxFileConfig*		pConfig;
public:
	virtual bool		OnInit();
	virtual int			OnExit();
protected:
	wxLocale			m_locale;
};

wxDECLARE_APP(CMainApp);

#endif


#pragma once

/*!
 * \file MainApp.h
 * \brief Application Class
 * \author Gianni.P
 * \version 0.1
 * \date 15/11/2014
 *
 * Logiciel de couplage de simulateur de vol avec un cockpit basé sur arduino
 *
 */

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

/*! \class CMainApp
 *  \brief classe d'application principale
 *
 *  Classe d'application de base wxwidgets avec gestion internationale
 */

class CMainApp : public wxApp
{
private:
	wxFileConfig*		pConfig; /*!< Fichier de configuration*/
public:
	/*!
	 *  \brief Initialisation
	 *
	 *  Méthode appelée à l'initialisation de l'application
	 */
	virtual bool		OnInit ();

	/*!
	 *  \brief Sortie
	 *
	 *  Méthode appelée à la sortie de l'application
	 */
	virtual int			OnExit();

protected:
	wxLocale			m_locale; /*!< Internationalisation*/
};

wxDECLARE_APP(CMainApp);

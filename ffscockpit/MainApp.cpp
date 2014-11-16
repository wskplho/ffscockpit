/*!
* \file MainApp.cpp
* \brief Application Class
* \author Gianni.P
* \version 0.1
* \date 15/11/2014
*
* Logiciel de couplage de simulateur de vol avec un cockpit bas� sur arduino
*
*/
#include "MainApp.h"

#include "MainFrame.h"
#include "Global.h"

#include "wx/stdpaths.h"
#include "wx/filename.h"

wxIMPLEMENT_APP(CMainApp);

bool CMainApp::OnInit()
{
	long lng = -1;
	wxString ConfPath = wxStandardPaths::Get().GetUserLocalDataDir() + wxFileName::GetPathSeparator();
	wxFileName ConfFile = ConfPath + AppName.Lower() + _T(".ini");
	if (!ConfFile.DirExists()) ConfFile.Mkdir(0777, wxPATH_MKDIR_FULL);
	pConfig = new wxFileConfig
		(
		AppName,
		wxEmptyString,
		ConfFile.GetLongPath(),
		wxEmptyString,
		wxCONFIG_USE_LOCAL_FILE
		);
	wxConfigBase::Set(pConfig);
	pConfig->Read(_T("misc/language"), &lng, -1);
	if ((lng < 0) || (lng >= NB_MAX_LNG))
	{
		lng = wxGetSingleChoiceIndex
			(
			_T("Please choose language:"),
			_T("Language"),
			WXSIZEOF(langNames),
			langNames
			);
	}
	if ((lng >= 0) || (lng < NB_MAX_LNG))
	{
		// don't use wxLOCALE_LOAD_DEFAULT flag so that Init() doesn't return
		// false just because it failed to load wxstd catalog
		if (!m_locale.Init(langIds[lng]))
		{
			wxLogError(_T("This language is not supported by the system."));
			return false;
		}
		pConfig->Write(_T("misc/language"), lng);
		pConfig->Flush();
	}
	else return false;

	// normally this wouldn't be necessary as the catalog files would be found
	// in the default locations, but when the program is not installed the
	// catalogs are in the build directory where we wouldn't find them by
	// default
#ifdef __WXMAC__
	wxFileName fname(wxTheApp->argv[0]);
	wxString path = fname.GetPath();
	wxLocale::AddCatalogLookupPathPrefix(path + _T("/../Resources/lng/"));
#else
	wxLocale::AddCatalogLookupPathPrefix(_T("./lng/"));
#endif
	// Initialize the catalogs we'll be using
	m_locale.AddCatalog(_T("backup"));

	// this catalog is installed in standard location on Linux systems and
	// shows that you may make use of the standard message catalogs as well
	//
	// if it's not installed on your system, it is just silently ignored
#ifdef __LINUX__
	{
		wxLogNull noLog;
		m_locale.AddCatalog(_T("fileutils"));
	}
#endif
	CMainFrame* MainFrame = new CMainFrame();
	MainFrame->Show(true);
	SetTopWindow(MainFrame);
	return true;
}

int CMainApp::OnExit()
{
	return 0;
}

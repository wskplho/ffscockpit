#ifndef GLOBAL_H
#define GLOBAL_H

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
	#include "wx/wx.h"
#endif

#include "wx/stdpaths.h"
#include "wx/filename.h"
#include "wx/version.h"

static const wxString AppName = _T("FFSCocpkit");
static const wxString AppRevision = AppName + _T(" V0.0.1");

static const wxString AppVersion = AppRevision;

static const long LongFileMax = 500000;

static const  int NbLigneMax = 150;

static const  wxString AppLicence = AppVersion
+	_T("\nBuild with ") + wxVERSION_STRING;

static wxString ConfPath;

#endif

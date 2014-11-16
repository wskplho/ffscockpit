#pragma once

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <vector>

#include "wx/timer.h"
#include "wx/config.h"

#include "wx/ctb/getopt.h"
#include "wx/ctb/iobase.h"
#include "wx/ctb/serport.h"
#include "wx/ctb/timer.h"
#include "Singleton.h"
#include "CPaquet.h"

using namespace std;

enum CSC_EVENTS
{
	CSC_UPDATE = wxID_HIGHEST
};

class CSerialCtrl : public CSingleton<CSerialCtrl>, public wxEvtHandler
{
	friend class CSingleton<CSerialCtrl>;
private:
						CSerialCtrl();
						~CSerialCtrl();
	wxTimer*			TUpdate;
	int					Baudrate;
	wxString			PortName;
	wxSerialPort*		Interface;
	bool				m_bConnected;
	BYTE				DataLen;
	vector<CPaquet>		Buffer;
	CPaquet				Data;
	wxConfigBase* 		pConfig;
	wxTextCtrl*			InView;
	wxTextCtrl*			OutView;

public:

	bool				IsConnected(){ return m_bConnected; }
	void				Connect(bool Flag = true);
	void				Send(CPaquet& Paquet);
	void				Save();
	bool				GetAvailable(CPaquet& Paquet);
	void				SetInView (wxTextCtrl* pTextCtrl);
	void				SetOutView (wxTextCtrl* pTextCtrl);

protected:
	void				OnUpdate(wxTimerEvent& event);
	wxDECLARE_EVENT_TABLE();
};

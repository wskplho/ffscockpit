#ifndef CSERIALCTRL_H
#define CSERIALCTRL_H

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
#include "CPaquet.h"

using namespace std;

enum CSC_EVENTS
{
	CSC_UPDATE = wxID_HIGHEST
};

class CSerialCtrl : public wxEvtHandler
{
private:
						CSerialCtrl();
						~CSerialCtrl();
static CSerialCtrl*		ptrCSerialCtrl;
	wxTimer*			TUpdate;
	int					Baudrate;
	wxString			PortName;
	wxSerialPort*		Interface;
	bool				m_bConnected;
	BYTE				DataLen;
	vector<CPaquet>		Buffer;
	CPaquet				Data;
	wxConfigBase* 		pConfig;

public:

	static CSerialCtrl* GetInstance();
	void				Destroy();

	bool				IsConnected(){ return m_bConnected; }
	void				Connect(bool Flag = true);
	void				Send(CPaquet& Paquet);
	void				Save();
	bool				GetAvailable(CPaquet& Paquet);

protected:
	void				OnUpdate(wxTimerEvent& event);
	wxDECLARE_EVENT_TABLE();
};

#endif
#ifndef CSIMCONNECT_H
#define CSIMCONNECT_H

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "SerialCtrl.h"
#include "SimConnect.h"

#define Bcd2Dec(BcdNum) HornerScheme(BcdNum,0x10,10)
#define Dec2Bcd(DecNum) HornerScheme(DecNum,10,0x10)

//Slave Codes
#define FFS_LOG  (UINT8)0x00
#define FFS_ADF  (UINT8)0x01
#define FFS_NAV  (UINT8)0x02
#define FFS_ALT  (UINT8)0x03
#define FFS_HB   (UINT8)0xFF

//Master Codes
#define FFS_NSL	(UINT8)0x01

struct  FSNAVFREQ
{
	UINT32 NAV1;
	UINT32 NAV2;
};

enum SC_EVENT
{
	SC_UPDATE = wxID_HIGHEST
};

static enum EVENT_ID{
	EVENT_SIM_START,
	EVENT_DN_FLAPS,
	EVENT_UP_FLAPS,
	EVENT_MENU,
	EVENT_MENU_COM,
	EVENT_NAV_INC,
	EVENT_NAV_BIG_INC,
	EVENT_NAV_DEC,
	EVENT_NAV_BIG_DEC,
	EVENT_NAV_SWITCH
};

static enum GROUP_ID {
	GROUP_A
};

static enum DATA_DEFINE_ID {
	DEFINITION_NAV
};

static enum DATA_REQUEST_ID {
	REQUEST_NAV
};

class CSimCon : public wxEvtHandler
{
public:
							CSimCon();
							~CSimCon();

	bool					IsConnected(){ return m_bConnected; }
	void					Connect(bool Flag = true);

private:
	CSerialCtrl*			SerialCtrl;
	UINT32					HornerScheme(UINT32 Num, UINT32 Divider, UINT32 Factor);
	bool					m_bConnected;
	HANDLE					hSimConnect;
	wxTimer*                TUpdate;

	void					Process(SIMCONNECT_RECV *pData, DWORD cbData);
static void	CALLBACK		DispatchCallback(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext);
	void					OnTUpdate(wxTimerEvent& event);
	wxDECLARE_EVENT_TABLE();
};

#endif
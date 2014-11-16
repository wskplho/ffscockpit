#include "SimCon.h"
#include "Global.h"
#include "CPaquet.h"

wxBEGIN_EVENT_TABLE(CSimCon, wxEvtHandler)

EVT_TIMER(SC_UPDATE, CSimCon::OnTUpdate)

wxEND_EVENT_TABLE()

CSimCon::CSimCon()
{
	m_bConnected = false;
	m_hSimConnect = NULL;
	SerialCtrl = CSerialCtrl::Instance();
	TUpdate = new wxTimer(this,SC_UPDATE);
}


CSimCon::~CSimCon()
{
	Connect(false);
}

void CSimCon::Connect(bool Flag)
{
	HRESULT hr;
	if ((!m_bConnected) && Flag)
	{
		if(SUCCEEDED (hr = SimConnect_Open (&m_hSimConnect, AppName, NULL, 0, 0, 0)))
		{
			hr = SimConnect_AddToDataDefinition (m_hSimConnect, DEFINITION_NAV, "NAV ACTIVE FREQUENCY:1", "Frequency BCD32", SIMCONNECT_DATATYPE_INT32);
			hr = SimConnect_AddToDataDefinition (m_hSimConnect, DEFINITION_NAV, "NAV STANDBY FREQUENCY:1", "Frequency BCD32", SIMCONNECT_DATATYPE_INT32);
			hr = SimConnect_AddToDataDefinition (m_hSimConnect, DEFINITION_ALT, "PLANE ALTITUDE", "feet", SIMCONNECT_DATATYPE_INT32);
			hr = SimConnect_MapClientEventToSimEvent (m_hSimConnect, EVENT_UP_FLAPS, "FLAPS_INCR");
			hr = SimConnect_MapClientEventToSimEvent (m_hSimConnect, EVENT_DN_FLAPS, "FLAPS_DECR");
			hr = SimConnect_SetNotificationGroupPriority (m_hSimConnect, GROUP_A, SIMCONNECT_GROUP_PRIORITY_HIGHEST);
			hr = SimConnect_MenuAddItem (m_hSimConnect, "ffscocpkit", EVENT_MENU, NULL);
			hr = SimConnect_MenuAddSubItem (m_hSimConnect, EVENT_MENU, "Config COM", EVENT_MENU_COM, NULL);
			hr = SimConnect_MapClientEventToSimEvent (m_hSimConnect, EVENT_NAV_INC, "NAV1_RADIO_FRACT_INC_CARRY");
			hr = SimConnect_MapClientEventToSimEvent (m_hSimConnect, EVENT_NAV_DEC, "NAV1_RADIO_FRACT_DEC_CARRY");
			hr = SimConnect_MapClientEventToSimEvent (m_hSimConnect, EVENT_NAV_BIG_INC, "NAV1_RADIO_WHOLE_INC");
			hr = SimConnect_MapClientEventToSimEvent (m_hSimConnect, EVENT_NAV_BIG_DEC, "NAV1_RADIO_WHOLE_DEC");
			hr = SimConnect_MapClientEventToSimEvent (m_hSimConnect, EVENT_NAV_SWITCH, "NAV1_RADIO_SWAP");
			// Request an event when the simulation starts
			hr = SimConnect_SubscribeToSystemEvent (m_hSimConnect, EVENT_SIM_START, "SimStart");
			
			SimConnect_RequestDataOnSimObject (m_hSimConnect, REQUEST_NAV, DEFINITION_NAV, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_SIM_FRAME, SIMCONNECT_DATA_REQUEST_FLAG_CHANGED);
			SimConnect_RequestDataOnSimObject (m_hSimConnect, REQUEST_ALT, DEFINITION_ALT, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_SECOND, SIMCONNECT_DATA_REQUEST_FLAG_CHANGED);
			m_bConnected = true;
			TUpdate->Start(200);
			wxLogMessage(_("Connected on flight simulator"));
		}
		else
		{
			wxLogMessage(wxString::Format(_("Connection to Sim fail : %d"), hr));
		}
	}
	else if (m_bConnected && (!Flag))
	{
		m_bConnected = false;
		SimConnect_Close (m_hSimConnect);
		TUpdate->Stop();
		wxLogMessage(_("Disconnected from flight simulator"));
	}
}

void CSimCon::Process(SIMCONNECT_RECV *pData, DWORD cbData)
{
	HRESULT hr;
	switch (pData->dwID)
	{
		case SIMCONNECT_RECV_ID_EVENT:
		{
			SIMCONNECT_RECV_EVENT *evt = (SIMCONNECT_RECV_EVENT*)pData;
			switch (evt->uEventID)
			{
				case EVENT_UP_FLAPS:
				case EVENT_DN_FLAPS:
				{
					wxLogMessage(wxString::Format(_T("Flaps Event Receive: %d"), evt->dwData));
					break;
				}
				case EVENT_MENU:
				{
					wxLogMessage(_T("Menu Event Triggered"));
					break;
				}
				case EVENT_MENU_COM:
				{
					wxLogMessage(_T("Menu COM Triggered"));
					break;
				}
				default:
				{
					wxLogMessage(wxString::Format(_T("Receive Event : %d"), evt->uEventID));
					break;
				}
			}
			break;
		}
		case SIMCONNECT_RECV_ID_SIMOBJECT_DATA:
		{
			SIMCONNECT_RECV_SIMOBJECT_DATA *pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA*)pData;
			switch (pObjData->dwRequestID)
			{
				case REQUEST_NAV:
				{
					float fNAV1, fNAV2;
					DWORD ObjectID = pObjData->dwObjectID;
					FSNAVFREQ *pS = (FSNAVFREQ*)&pObjData->dwData;
					fNAV1 = Bcd2Dec(pS->NAV1) / 10000.0;
					fNAV2 = Bcd2Dec (pS->NAV2) / 10000.0;
					//wxLogMessage (wxString::Format(_("Receive Nav1=%f Nav2=%f"),fNAV1,fNAV2));
					CPaquet Data;
					Data.AddByte ((UINT8)1); //Slave
					Data.AddByte(FFS_NAV); //Fonction
					Data.AddFloat(fNAV1);
					Data.AddFloat(fNAV2);
					SerialCtrl->Send(Data);
					break;
				}
				case REQUEST_ALT:
				{
					UINT32 iALT;
					DWORD ObjectID = pObjData->dwObjectID;
					iALT = pObjData->dwData;
					wxLogMessage (wxString::Format(_("Receive Alt=%i"),iALT));
					CPaquet Data;
					Data.AddByte ((UINT8)2); //Slave
					Data.AddByte (FFS_ALT); //Fonction
					Data.AddDWord (iALT);
					SerialCtrl->Send (Data);
					break;
				}
				default:
				{
					break;
				}
			}
			break;
		}
		case SIMCONNECT_RECV_ID_QUIT:
		{
			wxLogMessage(_T("Close Event Received"));
			Connect(false);
			break;
		}
		case SIMCONNECT_RECV_ID_EXCEPTION:
		{
			wxLogMessage(_("Exception received"));
			SIMCONNECT_RECV_EXCEPTION *except = (SIMCONNECT_RECV_EXCEPTION*)pData;
			break;
		}
		case SIMCONNECT_RECV_ID_OPEN:
		{
			wxLogMessage(_("Open Event received"));
			break;
		}
		default:
		{
			wxLogMessage(wxString::Format(_T("Received: %d"),pData->dwID));
			break;
		}
	}
}

UINT32 CSimCon::HornerScheme(UINT32 Num, UINT32 Divider, UINT32 Factor)
{
	UINT32 Remainder = 0, Quotient = 0, Result = 0;
	Remainder = Num%Divider;
	Quotient = Num / Divider;
	if (!(Quotient == 0 && Remainder == 0))
		Result += HornerScheme(Quotient, Divider, Factor)*Factor + Remainder;
	return Result;
}

void CALLBACK CSimCon::DispatchCallback(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext)
{
	CSimCon *pThis = reinterpret_cast<CSimCon*>(pContext);
	pThis->Process(pData, cbData);
}

void CSimCon::OnTUpdate(wxTimerEvent& WXUNUSED(event))
{
	::SimConnect_CallDispatch (m_hSimConnect, CSimCon::DispatchCallback, this);
	CPaquet Paquet;
	wxString Message;
	DWORD dwData;
	while (SerialCtrl->GetAvailable(Paquet))
	{
		Paquet.Begin ();
		wxString sData;
		EVENT_ID Event;
		float fData;
		INT32 iData;
		bool bData;
		HRESULT hr;
		UINT8 Slave;
		Paquet.GetByte(Slave);
		UINT8 Code;
		Paquet.GetByte(Code);
		switch (Code)
		{
			case FFS_LOG:
			{
				if(!Slave) Message = _ ("Master Log : ");
				else Message = wxString::Format (_T ("Slave n°%d Log : "), Slave);
				if (Paquet.GetString(sData)==PQ_OK) Message << sData;
				else Message << _ ("Error");
				wxLogMessage (Message);
				break;
			}
		case FFS_NAV:
		{
			Paquet.GetDWord (iData);
			bData = abs (iData)>100 ? true : false;
			if(iData == 0) Event = EVENT_NAV_SWITCH;
			else Event = iData>0 ? (bData ? EVENT_NAV_BIG_INC : EVENT_NAV_INC) : (bData ? EVENT_NAV_BIG_DEC : EVENT_NAV_DEC);
			hr = SimConnect_TransmitClientEvent(
				m_hSimConnect,
				SIMCONNECT_OBJECT_ID_USER,
				Event,
				0,
				SIMCONNECT_GROUP_PRIORITY_HIGHEST,
				SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY
				);
			break;
		}
		default:
		{
			break;
		}
		}
		Paquet.Clear();
	}
}
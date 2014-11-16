#include "SerialCtrl.h"

wxBEGIN_EVENT_TABLE(CSerialCtrl, wxEvtHandler)
	EVT_TIMER	(CSC_UPDATE,	CSerialCtrl::OnUpdate)
wxEND_EVENT_TABLE()

CSerialCtrl::CSerialCtrl()
{
	InView = NULL;
	OutView = NULL;
	Interface = new wxSerialPort();
	TUpdate = new wxTimer(this,CSC_UPDATE);
	Buffer.clear();
	Data.Clear();
	DataLen=0;
	pConfig = wxConfigBase::Get();
	pConfig->Read(_T("/Arduino/PortName"), &PortName,_T("com1"));
	pConfig->Read(_T("/Arduino/BaudRate"), &Baudrate, 115200);
	Save();

}

CSerialCtrl::~CSerialCtrl()
{
	Connect(false);
	Save();
}

void CSerialCtrl::Save()
{
	pConfig->Write(_T("/Arduino/PortName"), PortName);
	pConfig->Write(_T("/Arduino/BaudRate"), (int)Baudrate);
	pConfig->Flush();
}

void CSerialCtrl::Connect(bool Flag)
{
	if ((!m_bConnected) && Flag)
	{
		if (Interface->Open(PortName) < 0)
		{
			wxLogMessage(wxString::Format(_("Cannot Connect to port %s"), PortName));
			return;
		}
		else
		{
			Interface->SetBaudRate((wxBaud)Baudrate);
			m_bConnected = true;
			TUpdate->Start(200);
			wxLogMessage(_("Connected on Arduino Serial port"));
		}

	}
	else if (m_bConnected && (!Flag))
	{
		TUpdate->Stop();
		Interface->Close();
		m_bConnected = false;
		wxLogMessage(_("Disconnected from Arduino Serial port"));
	}
}

void CSerialCtrl::OnUpdate(wxTimerEvent& event)
{
	unsigned char byte;
	while (Interface->Read((char*)(&byte), 1))
	{
		if (DataLen < 1)
		{
			DataLen = byte;
			if (InView) InView->AppendText (wxString::Format (_T ("\n%02X:"), byte));
		}
		else
		{
			Data.AddByte(byte);
			if(InView) InView->AppendText (wxString::Format (_T ("%02X:"), byte));
			DataLen --;
			if (DataLen < 1)
			{
				Buffer.push_back(Data);
				Data.Clear();
			}
		}
	}
	event.Skip();
}

void CSerialCtrl::Send(CPaquet& Paquet)
{
	if (!m_bConnected) return;
	UINT8 Taille = Paquet.GetSize ();
	if (Taille<1) return;
	char* Pile = new char[Taille+1];
	Pile[0]= Taille;
	Paquet.Get((UINT8*)Pile+1,Taille);
	if (OutView)
	{
		wxString Datagram;
		OutView->AppendText (wxString::Format (_T ("\n%02X:"), Taille));
		Paquet.Get (Datagram);
		OutView->AppendText (Datagram);
	}
	Interface->Write(Pile, Taille +1);
}

bool CSerialCtrl::GetAvailable(CPaquet& Paquet)
{
	Paquet.Clear ();
	if (Buffer.empty()) return false;
	vector<CPaquet>::iterator it = Buffer.begin ();
	Paquet = *it;
	Buffer.erase(it);
	return true;
}

void CSerialCtrl::SetInView (wxTextCtrl* pTextCtrl)
{
	InView = pTextCtrl;
}

void CSerialCtrl::SetOutView (wxTextCtrl* pTextCtrl)
{
	OutView = pTextCtrl;
}
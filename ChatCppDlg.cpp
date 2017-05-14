// ChatCppDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ChatCppDlg.h"
#include ".\chatcppdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

bool access = true;

CChatCppDlg::CChatCppDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChatCppDlg::IDD, pParent)
	, password(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChatCppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO_CLIENT, m_ButtonStartClient);
	DDX_Control(pDX, IDC_EDIT_NAME, m_wndName);
	DDX_Control(pDX, IDC_EDIT_IP, m_wndIPAddress);
	DDX_Control(pDX, IDC_EDIT_PORT, m_wndPort);
	DDX_Control(pDX, IDC_EDIT_SEND, m_wndSend);
	DDX_Control(pDX, IDC_BUTTON_SEND, m_ButtonSend);
	DDX_Control(pDX, IDC_BUTTON_STOPCHAT, m_ButtonStopChat);
	DDX_Control(pDX, IDC_EDIT_CHAT, m_wndChat);
	DDX_Text(pDX, IDC_EDIT1, password);
}

BEGIN_MESSAGE_MAP(CChatCppDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_RADIO_CLIENT, OnBnClickedRadioClient)
	ON_BN_CLICKED(IDC_BUTTON_STOPCHAT, OnBnClickedButtonStopchat)
	ON_BN_CLICKED(IDC_BUTTON_SEND, OnBnClickedButtonSend)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

int key = 0;

BOOL CChatCppDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	
	//////////////////////////////////////////////////////////////////////////////
	// ����������� �������������
	SetWindowText("��� ����!");

	m_wndName.SetLimitText(12); // ����������� ���������� �������� ��������.
	
	m_wndSend.SetLimitText(200); // ����������� ���������� �������� ��������.
	m_wndIPAddress.SetWindowText("127.0.0.1"); // ����� �� ��������� ��� ������������.
	m_wndPort.SetWindowText("8000");
	m_ButtonStopChat.SetWindowText(g_strExitFromChat);
	m_mainSocket.m_pParent = this;

	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CChatCppDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

CString encode(CString s)
{
	std::string s1((LPCSTR)s);
	std::string out;
	for (unsigned int i = 0;i < s1.length();i++)
		out += (char)(s[i] + 120) % 125 + 12;
	return CString(out.c_str());
}


// ��������� �������
void CChatCppDlg::OnBnClickedRadioClient()
{
	UpdateData(true);
	CString S = "fdsssdf";
	if (encode(password) != S)
	{
		AfxMessageBox(_T("Error password"));
		return;
	}
	// �������� �������������������� ������� �������:
	// ���� ������ �� � ��������� �������,
	// ���� ����� � ������ (�.�. ������ � ������� ������� ����� �������� ��������),
	// ����� �������� ���� � ���� ��� ����� ��� ����������� �������..
	if(m_ButtonStartClient.GetCheck() != BST_CHECKED) return;
	if(m_mainSocket.m_hSocket != INVALID_SOCKET) return;
	if(QueryName() == false)
	{
		AfxMessageBox("������� ���� ��� ��� ����!");
		StopChat();
		return;
	}


	// ����� CAsyncSocket �������� ��������� �������� ������,
	// ��������� � ������� Create() ���������������� �������� 
	// ������ � ���������� ��� � ����� �� IP ������� ��������� �� ����������.
	if(m_mainSocket.Create() == TRUE)
	{
		CString strAddress;
		m_wndIPAddress.GetWindowText(strAddress);
		CString strPort;
		m_wndPort.GetWindowText(strPort);
		if(m_mainSocket.Connect(strAddress, atoi(strPort)) == FALSE)
		{
			// � ������������ ������ ��� ���� ������
			// ��������� ��� �������� ������� �����������,
			// �.�. ����������� �������� �������.
			if(GetLastError() == WSAEWOULDBLOCK)
			{
				DisabledControl(false);
			}
			else
			{
				// ���� �����-���� ������ ��������,
				// �������� ���������� � �������������� ���������,
				// ������� � ��������� ������� �������� ����������.
				StopChat();
			}
		}
	}
}


// ������ ������ "����� �� ����".
void CChatCppDlg::OnBnClickedButtonStopchat()
{
	StopChat();
}

// ��������� ������ � ����������� ��� ������ 
// ���������� � ������ ������� ��� �������.
// ���� ������� - �������� ���������� �� 
// ���������� ������� "������������" ������.
void CChatCppDlg::DisabledControl(bool server)
{
	// �������.
	m_wndIPAddress.EnableWindow(FALSE);
	m_wndPort.EnableWindow(FALSE);
	m_ButtonSend.EnableWindow(FALSE);

	if(server == true)
	{
		m_ButtonStopChat.SetWindowText(g_strStopChat);
		m_ButtonStartClient.EnableWindow(TRUE);
	}
	else
	{
		m_ButtonStopChat.SetWindowText(g_strExitFromChat);
		//m_ButtonStartServer.EnableWindow(FALSE);
	}

	// ����������.
	// ��������� ����������� ������ �� ����.
	m_ButtonStopChat.EnableWindow(TRUE);
}

// ��������� ������ � ����������� ����� �������� �������.
// ���� ������� - �������� ���������� �� 
// ���������� ������� "������������" ������.
void CChatCppDlg::EnabledControl(void)
{
	// ����������.
	m_wndIPAddress.EnableWindow(TRUE);
	m_wndPort.EnableWindow(TRUE);
	m_ButtonStartClient.EnableWindow(TRUE);
	//m_ButtonStartServer.EnableWindow(TRUE);
	

	// �������.
	m_ButtonStopChat.EnableWindow(FALSE);
	m_ButtonSend.EnableWindow(FALSE);
}


// ����� �� ����,
// ���� ��� ��������� �� ������� �������,
// �� ��� ������ ��������� ����.
// ��� ����� �������� ������ ����, �� ����
// ����������� ������ ���� �������� ����������� �������...
void CChatCppDlg::StopChat(void)
{
	// �������� ������ �� ���������� �� ����.
	SendDisconnect();

	m_mainSocket.Close();
	// ������� ������ �� �������� ���������.
	m_vecSockets.clear();

	//m_ButtonStartServer.SetCheck(BST_UNCHECKED);
	m_ButtonStartClient.SetCheck(BST_UNCHECKED);

	// �������� ������ � ���������� ���
	// ��������� �������.
	EnabledControl();

	// � ���� ��� ������.
	SetWindowText("��� ����!");
}


// �������� ��������������� ���������.
void CChatCppDlg::OnBnClickedButtonSend()
{
	CString strChat;
	m_wndSend.GetWindowText(strChat);
	SendChat(strChat);

}

// ���������� ��������� �� ���� ����.
void CChatCppDlg::OnReceive(void)
{
	SENDBUFFER sb;
	if(m_ButtonStartClient.GetCheck() == BST_CHECKED)
	{
		m_mainSocket.Receive(&sb, sizeof(SENDBUFFER));
	}

	// ��������� ��������� ��������� �� ������
	// ��� ����.
	switch(sb.typemessage)
	{
	case m_TypeMessage::tmChat:
		{
			CString strChat;
			m_wndChat.GetWindowText(strChat);
			strChat += "    " + CString(sb.name) + ": " + CString(sb.buffer) + "\r\n";
			m_wndChat.SetWindowText(strChat);
			int number_line = m_wndChat.GetLineCount();
			m_wndChat.LineScroll(number_line);
		}
		break;
	case m_TypeMessage::tmDisconnect:
		{
			CString strChat;
			m_wndChat.GetWindowText(strChat);
			// ���� ������� ��������� �� ��������� ����(���������� �������),
			// ������� ���������� ���������.
			strChat += "    " + CString(sb.name) + " - �������(�) ���!" + "\r\n";
			m_wndChat.SetWindowText(strChat);
			int number_line = m_wndChat.GetLineCount();
			m_wndChat.LineScroll(number_line);
		}
		break;
	}
}

// ��� �������� ���������� �������� � ���
// ���������� �� ���������� ���������.
void CChatCppDlg::OnClose()
{
	StopChat();
	CDialog::OnClose();
}
// ������� ������-��������� � ���.
void CChatCppDlg::SendChat(CString strMessage)
{
	SENDBUFFER sb;
	int len = strMessage.GetLength();
	//strMessage = encod(std::string((LPCTSTR)strMessage), len);
	memcpy(sb.buffer, strMessage.GetBuffer(), sizeof(TCHAR)*len);
	m_wndName.GetWindowText(strMessage);
	len = strMessage.GetLength();
	memcpy(sb.name, strMessage.GetBuffer(), sizeof(TCHAR)*len);
	sb.typemessage = m_TypeMessage::tmChat;
	SendBuffer(sb, true);
}

// ������� ����� ��������������� ��������� � ����.
void CChatCppDlg::SendBuffer(SENDBUFFER sb, bool toserver)
{
	// ���� ����� �� ������, ������ ������ � ���� �������.
	if(m_mainSocket.m_hSocket == INVALID_SOCKET) return;
	if(m_ButtonStartClient.GetCheck() == BST_CHECKED)
	{
		int send = m_mainSocket.Send(&sb, sizeof(SENDBUFFER));
		if(send == sizeof(SENDBUFFER))
			m_wndSend.SetWindowText("");
	}
}


// ��������� � ���������� ��������� �� ���������� �� ����.
void CChatCppDlg::SendDisconnect(void)
{
	SENDBUFFER sb;
	CString s;
	m_wndName.GetWindowText(s);
	int len = s.GetLength();
	memcpy(sb.name, s.GetBuffer(), sizeof(TCHAR)*len);
	sb.typemessage = m_TypeMessage::tmDisconnect;
	SendBuffer(sb, true);

}

// ������� �����������, ���������� �� ������� �������.
void CChatCppDlg::OnConnect(BOOL Error)
{
	if(Error == TRUE)
	{
		AfxMessageBox("������� ����������� ���� ����������!\n�������� ������ ��� �� ������!");
		StopChat();
	}
	else
	{
		m_ButtonSend.EnableWindow(TRUE);
		SetWindowText("���� ��������!");
		TCHAR key_out[1];
		ZeroMemory(key_out, sizeof(TCHAR));
		m_mainSocket.Receive(&key_out, sizeof(key_out));
		CString str = key_out;
		key = atoi(str);
	}
}

// ������ ����� ��������� ����� ��������� ������.
bool CChatCppDlg::QueryName(void)
{
	CString strName;
	m_wndName.GetWindowText(strName);
	if(strName == g_EmptyName || strName.IsEmpty() == true)
		return false;

	return true;
}

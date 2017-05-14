// ChatCppDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "ChatCpp.h"
#include "Sock.h"
#include "vector"


const CString g_EmptyName = "���� ���?";
const CString g_strStopChat = "���������� ���";
const CString g_strExitFromChat = "����� �� ����";

struct SENDBUFFER
{
	SENDBUFFER() 
	{
		key = 0;
		typemessage = 0; 
		ZeroMemory(name, sizeof(TCHAR)*14); 
		ZeroMemory(buffer, sizeof(TCHAR)*202);
	}
	int key;
	int typemessage;
	TCHAR name[14];
	TCHAR buffer[202];
};



// CChatCppDlg dialog
class CChatCppDlg : public CDialog
{
// Construction
public:
	CChatCppDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CHATCPP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadioClient();
	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnBnClickedButtonStopchat();
	afx_msg void OnClose();
	void OnOK() {}
	
	CString IntToStr(int number)
	{
		CString str;
		str.Format("%d", number);
		return str;
	}

	void OnConnect(BOOL Error);
	void OnReceive(void);

private:

	// ��������� ������ � �����������
	void DisabledControl(bool server);
	// ��������� ������ � ����������� ����� �������� �������
	void EnabledControl(void);
	void StopChat(void);
	// ������� ��������� � ���.
	void SendChat(CString strMessage);
	void SendBuffer(SENDBUFFER sb, bool toserver);
	void SendDisconnect(void);

	
	CButton m_ButtonSend;
	CButton m_ButtonStartClient;
	CButton m_ButtonStopChat;
	CEdit m_wndChat;
	CEdit m_wndName;
	CEdit m_wndIPAddress;
	CEdit m_wndPort;
	CEdit m_wndSend;

	CSock m_mainSocket;
	std::vector<CSock*> m_vecSockets;
	enum m_TypeMessage {tmCountPeople=1, tmChat, tmDisconnect};

	// ������ ����� ���������.
	bool QueryName(void);
public:
	CString password;
};

#include "stdafx.h"
#include "ChatCppDlg.h"
#include "Sock.h"




CSock::CSock()
: m_pParent(NULL)
{
	
}


CSock::~CSock()
{
}

// ������� ����������� �� ������� �������� ��� ����������.
void CSock::OnConnect(int nErrorCode)
{
	CChatCppDlg* pDlg = (CChatCppDlg*)m_pParent;


	nErrorCode == 0 ? pDlg->OnConnect(FALSE) : pDlg->OnConnect(TRUE);
	

	CAsyncSocket::OnConnect(nErrorCode);
}

// ������� ���������� �� ����
void CSock::OnClose(int nErrorCode)
{
}

// ������� ����������� ��������� ���������.
void CSock::OnReceive(int nErrorCode)
{

	CChatCppDlg* pDlg = (CChatCppDlg*)m_pParent;
	pDlg->OnReceive();
	
	CAsyncSocket::OnReceive(nErrorCode);
}


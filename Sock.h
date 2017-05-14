#pragma once
#include <afxsock.h>		// MFC socket extensions



// ����� ������� ����������� �� ������ 
// ����������� ������� CAsyncSocket.
class CSock : public  CAsyncSocket
{
public:
	CSock();
	virtual ~CSock();
	
	// ������� ��� ����������� ���������
	// ��������� � ����������� ������.
	virtual void OnConnect(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);

	// ���������� ��� "�����" � ������������ �����.
	CWnd* m_pParent;
};



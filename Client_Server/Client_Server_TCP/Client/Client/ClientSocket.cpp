// ClientSocket.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Client.h"
#include "ClientSocket.h"
#include "ClientDlg.h"


// ClientSocket

ClientSocket::ClientSocket()
{
}

ClientSocket::~ClientSocket()
{
}


// ClientSocket ��Ա����


void ClientSocket::OnConnect(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���

	CAsyncSocket::OnConnect(nErrorCode);
}


void ClientSocket::OnReceive(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���

	CClientDlg *dlg=(CClientDlg*)AfxGetApp()->GetMainWnd();//ȡ��������ָ�����

	//receive respond

	TCHAR respond[100];  
	Receive(respond,100,0);

	CString time(respond);
	dlg->m_respond=time;

	dlg->UpdateData(false);


	CAsyncSocket::OnReceive(nErrorCode);
}


void ClientSocket::OnSend(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���

		
	CAsyncSocket::OnSend(nErrorCode);
}

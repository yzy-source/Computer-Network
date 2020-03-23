// ClientSocket.cpp : 实现文件
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


// ClientSocket 成员函数


void ClientSocket::OnConnect(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类

	CAsyncSocket::OnConnect(nErrorCode);
}


void ClientSocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类

	CClientDlg *dlg=(CClientDlg*)AfxGetApp()->GetMainWnd();//取得主窗口指针对象

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
	// TODO: 在此添加专用代码和/或调用基类

		
	CAsyncSocket::OnSend(nErrorCode);
}

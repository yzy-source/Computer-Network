// ClientSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "ClientSocket.h"
#include "afxdialogex.h"
#include "ClientDlg.h"


// ClientSocket

ClientSocket::ClientSocket()
{
}

ClientSocket::~ClientSocket()
{
}


// ClientSocket 成员函数


void ClientSocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	CClientDlg *dlg=(CClientDlg*)AfxGetApp()->GetMainWnd();//取得主窗口指针对象

	//receive respond

	TCHAR respond[100];  
	ReceiveFrom(respond,100,dlg->m_ip,dlg->m_port);

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

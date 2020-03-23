// ListenServer.cpp : 实现文件
//

#include "stdafx.h"
#include "SMTPServer.h"
#include "ListenServer.h"
#include "ServerSocket.h"
#include "SMTPServerDlg.h"

// ListenServer

ListenServer::ListenServer()
{
}

ListenServer::~ListenServer()
{
}


// ListenServer 成员函数


void ListenServer::OnAccept(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	//监听之后Accept,处理连接请求
	CSMTPServerDlg* dlg=(CSMTPServerDlg*)AfxGetApp()->GetMainWnd(); //获得主窗口指针对象
	dlg->m_log.AddString (L"收到连接请求……");
	ServerSocket *socket=new ServerSocket();
	if(Accept(*socket))
	{
		dlg->m_log.AddString(L"正在建立连接……");
		socket->AsyncSelect(FD_WRITE);//触发socket的Onsend()函数
	}
	else
		delete socket;

	CAsyncSocket::OnAccept(nErrorCode);
}

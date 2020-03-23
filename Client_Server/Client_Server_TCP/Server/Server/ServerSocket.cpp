// ServerSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "Server.h"
#include "ServerSocket.h"
#include "ServerDlg.h"


// ServerSocket

ServerSocket::ServerSocket()
	:  port(2001)
{
	char    HostName[100];
    gethostname(HostName, sizeof(HostName));// 获得本机主机名.

    hostent* hn;
    hn = gethostbyname(HostName);//根据本机主机名得到本机ip

   
	IP=inet_ntoa(*(struct in_addr *)hn->h_addr_list[0]);//把ip换成字符串形式
    
}

ServerSocket::~ServerSocket()
{
}


// ServerSocket 成员函数


void ServerSocket::OnAccept(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	ServerSocket* myserver = new ServerSocket;
	Accept (*myserver);
	myserver->GetPeerName (myserver->IP,myserver->port);

	CAsyncSocket::OnAccept(nErrorCode);
}


void ServerSocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类

	CServerDlg *dlg=(CServerDlg*)AfxGetApp()->GetMainWnd();//取得主窗口指针对象
	
	
	//获得客户端请求
	Receive(request,100,0);

	CString Date=CTime::GetCurrentTime().Format(L"%Y/%m/%d");           //get 当前时间
	CString Time=CTime::GetCurrentTime().Format(L"%H:%M:%S");

	CString respond;            //响应

	CString req(request);      //获得请求字符串
	req.MakeLower ();          //忽略大小写
	if(req=="date")
		respond=Date;
	else 
		if(req=="time")
			respond=Time;
		else
			respond="Error request";

	//send respond

	Send(respond.GetBuffer(),(respond.GetLength()+1)*sizeof(TCHAR),0);

	//记录日志
	CString logstr;
	logstr=Date+L" "+Time+L" 收到IP="+IP+L" Port=2001"+L"请求【"+req+L"】,响应【"+respond+L"】"; 
	dlg->list.AddString(logstr);


	CAsyncSocket::OnReceive(nErrorCode);
}

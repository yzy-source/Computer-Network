// ServerSocket.cpp : ʵ���ļ�
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
    gethostname(HostName, sizeof(HostName));// ��ñ���������.

    hostent* hn;
    hn = gethostbyname(HostName);//���ݱ����������õ�����ip

   
	IP=inet_ntoa(*(struct in_addr *)hn->h_addr_list[0]);//��ip�����ַ�����ʽ
    

}

ServerSocket::~ServerSocket()
{
}


// ServerSocket ��Ա����


void ServerSocket::OnReceive(int nErrorCode)
{
	// TODO: �ڴ�����ר�ô����/����û���

	CServerDlg *dlg=(CServerDlg*)AfxGetApp()->GetMainWnd();//��� ������ָ�����
	
	
	//��ÿͻ�������
	ReceiveFrom(request,100,IP,port,0);

	CString Date=CTime::GetCurrentTime().Format(L"%Y/%m/%d");           //get ��ǰʱ��
	CString Time=CTime::GetCurrentTime().Format(L"%H:%M:%S");

	CString respond;            //��Ӧ

	CString req(request);      //��������ַ���
	req.MakeLower ();          //���Դ�Сд
	if(req=="date")
		respond=Date;
	else 
		if(req=="time")
			respond=Time;
		else
			respond="Error request";

	SendTo(respond.GetBuffer(),(respond.GetLength()+1)*sizeof(TCHAR),port,IP,0);

	//��¼��־
	CString logstr;
	logstr=Date+L" "+Time+L" �յ�IP="+IP+L" Port=2001"+L"����"+req+L"��,��Ӧ��"+respond+L"��"; 
	dlg->list.AddString(logstr);


	CAsyncSocket::OnReceive(nErrorCode);
}
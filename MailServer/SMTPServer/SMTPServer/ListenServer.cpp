// ListenServer.cpp : ʵ���ļ�
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


// ListenServer ��Ա����


void ListenServer::OnAccept(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
	//����֮��Accept,������������
	CSMTPServerDlg* dlg=(CSMTPServerDlg*)AfxGetApp()->GetMainWnd(); //���������ָ�����
	dlg->m_log.AddString (L"�յ��������󡭡�");
	ServerSocket *socket=new ServerSocket();
	if(Accept(*socket))
	{
		dlg->m_log.AddString(L"���ڽ������ӡ���");
		socket->AsyncSelect(FD_WRITE);//����socket��Onsend()����
	}
	else
		delete socket;

	CAsyncSocket::OnAccept(nErrorCode);
}

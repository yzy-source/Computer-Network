// ServerSocket.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SMTPServer.h"
#include "ServerSocket.h"
#include "SMTPServerDlg.h"

#include<string>
#include <regex>
#include <sstream>
#include <fstream>
// ServerSocket

ServerSocket::ServerSocket()
	: end(false)
	, data(false)
	, length(0)
	, revtext(_T(""))
{
	memset(Buffer,0,sizeof(Buffer));
	revtext.Empty();
}

ServerSocket::~ServerSocket()
{
	if(m_hSocket!=INVALID_SOCKET)
	{
		Close();
	}
}


// ServerSocket ��Ա����


void ServerSocket::OnReceive(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
	CSMTPServerDlg *dlg=(CSMTPServerDlg*)AfxGetApp()->GetMainWnd();//������ָ�����
	memset(Buffer,0,sizeof(Buffer));  //ÿ��receive��֮ǰ��Ҫ�ѻ���������
	length=Receive(Buffer,sizeof(Buffer),0);//�����յ���Ϣ�ĳ��ȣ��յ�����Ϣ����Buffer��
	
	CString str(Buffer);//�������е�ת�����ַ���
    while(data&&!end)     //����յ����ݣ�δ������˵������������
	{
		revtext+=str;  
		if(str.Right(5)!="\r\n.\r\n") 
			return;
		else
			end=true;         //�����յ�\r\n.\r\n ������־

	}
	if(str.Left(4)=="HELO"||str.Left (4)=="EHLO")
	{
		dlg->m_log.AddString(L"C:"+str);
		char reply[100]="250 hello\r\n";
		Send(reply,strlen(reply));
		dlg->m_log.AddString(L"S:250 OK 127.0.0.1");
		AsyncSelect(FD_READ);
	}
	if(str.Left(10)=="MAIL FROM:")
	{
		dlg->m_log.AddString(L"C:"+str);
		char reply[100]="250 mail from\r\n";
		Send(reply,strlen(reply));
		dlg->m_log.AddString(L"S:250 OK Get Sender");
		AsyncSelect(FD_READ);
	}
	if(str.Left(8)=="RCPT TO:")
	{
		dlg->m_log.AddString(L"C:"+str);
		char reply[100]="250 receiver\r\n";
		Send(reply,strlen(reply));
		dlg->m_log.AddString(L"S:250 OK Get Receiver");
		AsyncSelect(FD_READ);
	}
	if(str.Left(4)=="DATA")
	{
		dlg->m_log.AddString(L"C:"+str);
		data=true; //�ʼ����Ŀ�ʼ��־
		char reply[100]="354 data\r\n";
		Send(reply,strlen(reply));
		dlg->m_log.AddString(L"S:354 Go Ahead!End With <CRLF>.<CRLF>");
		dlg->m_log.AddString(L"S:250 Message Accepted For Delivery!");
		AsyncSelect(FD_READ);
		return;
	}	

	if(data&&end)
	{
		char temp[98000]={'\0'};
		bool content=false;
		int tag=0;//��¼����base64֮��ڼ������������س�����
		//��һ��base64����֮�������س�����֮�󵽡�------������ǰ�����س�����֮��Ĳ��������ĵı���
		CString content_all;//�洢�ʼ����ĵĽ���
		content_all.Empty();

		for(int i=0;i<revtext.GetLength()-1;i++)
		{
			int k=0;
			CString t(temp);
			if(t.Right(6)=="base64"&&tag==0)//�õ����ı���
				content=true;
			memset(temp,0,strlen(temp));
			while(!(revtext[i+1]=='\n'))//�õ�һ�����ݴ浽temp
				temp[k++]=revtext[i++];
			
			CString buff(temp);
			dlg->m_message.AddString(buff);
			if(content)
				tag++;     //�����س����м�¼

			buff.Remove('\n');
			if(buff.Left(6)==L"------")
					content=false;   //���Ľ�����־
			if(content&&tag!=1)
				content_all=content_all+buff; 
		}
		int len;
		CString decodeContent=dlg->Decodetext(content_all,&len); //������base64����
		int start=0,end;//������ʾ�ʼ�����
		end=decodeContent.Find('\n',start);//δ�ҵ�����-1
		while(end!=-1)   
		{
			CString temp=decodeContent.Mid(start,end-start);//��\n֮ǰ��string
			dlg->m_text.AddString(temp);//��ʾÿһ���ʼ�����
			start=end+1;  //��������һ��
			end=decodeContent.Find('\n',start);
		}

		//�����Ĵ���
		int begin,last;
		begin=revtext.Find(L"filename");//filename����֮���һ��˫�������������Ǹ�����
		if(begin!=-1)//�и���
		{
			last=revtext.Find('"',begin);//filename��ʼ
			begin=revtext.Find('"',last+1);//filename����

			CString filename;
			filename.Empty();
			filename=revtext.Mid(last+1,begin-last-1);//������
			dlg->m_name=filename;
			dlg->UpdateData(false); 

			last=revtext.Find(L"------",begin);//������֮��------����֮ǰ�����ݳ�ȥ�س����о��Ǹ����ı���
			CString fileContent=revtext.Mid(begin+5,last-(begin+5)); //����\r\n\r\n
			
			std::vector<char> decodeFileContent;
			fileContent.Remove('\r');
			fileContent.Remove('\n');
			dlg->DecodePicture(fileContent,decodeFileContent);//�������ݵĽ���

			//��������д�������ļ�
			std::fstream fout(filename, std::ios_base::out | std::ios_base::binary);;
			fout.write(static_cast<const char*>(&decodeFileContent[0]), decodeFileContent.size());
			fout.close();

			//ͼƬ��ʾ
			CImage image;
			image.Load(filename);
			if (!image.IsNull()) 
			{
				SetStretchBltMode(dlg->m_attach.GetDC()->GetSafeHdc(), HALFTONE);

				int weight=image.GetWidth();
				int height=image.GetHeight();
				 // �ҳ���͸��еĽϴ�ֵ��
				int Sourcemax=(weight>height)?weight:height;
				CRect dest;
				dlg->m_attach.GetClientRect(&dest);
				int destmax=(dest.Width()<dest.Height())?dest.Height():dest.Width();
				// ���㽫ͼƬ���ŵ�TheImage��������ı�������
				float scale = (float) ( (float) Sourcemax / (float)destmax );
    
				// ���ź�ͼƬ�Ŀ�͸�
				int neww = (int)( weight/scale );
				int newh= (int)( height/scale );

				// Ϊ�˽����ź��ͼƬ�������в�λ�������ͼƬ�� rectDraw ���Ͻǵ���������ֵ
				int tlx = (neww > newh)? 0: (int)(dest.Width()-neww)/2;
				int tly = (neww > newh)? (int)(dest.Height()-newh)/2: 0;

				// ���� rectDraw,��������ͼƬimage
				CRect rect = dest;
				rect.SetRect(tlx, tly, neww, newh);

				image.Draw(dlg->m_attach.GetDC()->GetSafeHdc(), rect);
				
			}
		}

		char buff[100]="250 data\r\n";
		Send(buff,strlen(buff));//��������Ӧ250����ʾ���������ɹ����
		dlg->m_log.AddString(L"S:250  Message accepted for delivery!");
		data=false;
		AsyncSelect(FD_READ); //����ͨ��socket��OnReceive���� 
		return;

	}
	
	if(str.Left(4)=="QUIT")
	{
		dlg->m_log.AddString(L"C:"+str);
		char reply[100]="221 succeed\r\n";
		Send(reply,strlen(reply));
		dlg->m_log.AddString(L"S:221 Quit! Goodbye!");
		AsyncSelect(FD_READ);
	}
	CAsyncSocket::OnReceive(nErrorCode);
}


void ServerSocket::OnSend(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
	CSMTPServerDlg *dlg=(CSMTPServerDlg*)AfxGetApp()->GetMainWnd();//������ָ�����
	char buffer[100]= "220 ready\r\n";
	Send(buffer,strlen(buffer));//��Ӧ220������
	dlg->m_log.AddString(L"S:220 Ready For Mail Service");
	AsyncSelect(FD_READ);//����OnReceive�������������
	CAsyncSocket::OnSend(nErrorCode);
}

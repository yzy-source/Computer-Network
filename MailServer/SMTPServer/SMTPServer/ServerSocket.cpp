// ServerSocket.cpp : 实现文件
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


// ServerSocket 成员函数


void ServerSocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	CSMTPServerDlg *dlg=(CSMTPServerDlg*)AfxGetApp()->GetMainWnd();//主窗口指针对象
	memset(Buffer,0,sizeof(Buffer));  //每次receive的之前需要把缓冲区清零
	length=Receive(Buffer,sizeof(Buffer),0);//返回收到消息的长度，收到的信息放在Buffer中
	
	CString str(Buffer);//缓冲区中的转化成字符串
    while(data&&!end)     //如果收到数据，未结束，说明缓冲区不够
	{
		revtext+=str;  
		if(str.Right(5)!="\r\n.\r\n") 
			return;
		else
			end=true;         //连续收到\r\n.\r\n 结束标志

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
		data=true; //邮件正文开始标志
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
		int tag=0;//记录遇到base64之后第几次遇到两个回车换行
		//第一个base64出现之后两个回车换行之后到“------”出现前两个回车换行之间的部分是正文的编码
		CString content_all;//存储邮件正文的解码
		content_all.Empty();

		for(int i=0;i<revtext.GetLength()-1;i++)
		{
			int k=0;
			CString t(temp);
			if(t.Right(6)=="base64"&&tag==0)//得到正文编码
				content=true;
			memset(temp,0,strlen(temp));
			while(!(revtext[i+1]=='\n'))//得到一行数据存到temp
				temp[k++]=revtext[i++];
			
			CString buff(temp);
			dlg->m_message.AddString(buff);
			if(content)
				tag++;     //遇到回车换行记录

			buff.Remove('\n');
			if(buff.Left(6)==L"------")
					content=false;   //正文结束标志
			if(content&&tag!=1)
				content_all=content_all+buff; 
		}
		int len;
		CString decodeContent=dlg->Decodetext(content_all,&len); //对正文base64解码
		int start=0,end;//分行显示邮件正文
		end=decodeContent.Find('\n',start);//未找到返回-1
		while(end!=-1)   
		{
			CString temp=decodeContent.Mid(start,end-start);//找\n之前的string
			dlg->m_text.AddString(temp);//显示每一行邮件正文
			start=end+1;  //接着找下一行
			end=decodeContent.Find('\n',start);
		}

		//附件的处理
		int begin,last;
		begin=revtext.Find(L"filename");//filename出现之后第一个双引号括起来的是附件名
		if(begin!=-1)//有附件
		{
			last=revtext.Find('"',begin);//filename开始
			begin=revtext.Find('"',last+1);//filename结束

			CString filename;
			filename.Empty();
			filename=revtext.Mid(last+1,begin-last-1);//附件名
			dlg->m_name=filename;
			dlg->UpdateData(false); 

			last=revtext.Find(L"------",begin);//”出现之后------出现之前的内容除去回车换行就是附件的编码
			CString fileContent=revtext.Mid(begin+5,last-(begin+5)); //包括\r\n\r\n
			
			std::vector<char> decodeFileContent;
			fileContent.Remove('\r');
			fileContent.Remove('\n');
			dlg->DecodePicture(fileContent,decodeFileContent);//附件内容的解码

			//附件解码写到磁盘文件
			std::fstream fout(filename, std::ios_base::out | std::ios_base::binary);;
			fout.write(static_cast<const char*>(&decodeFileContent[0]), decodeFileContent.size());
			fout.close();

			//图片显示
			CImage image;
			image.Load(filename);
			if (!image.IsNull()) 
			{
				SetStretchBltMode(dlg->m_attach.GetDC()->GetSafeHdc(), HALFTONE);

				int weight=image.GetWidth();
				int height=image.GetHeight();
				 // 找出宽和高中的较大值者
				int Sourcemax=(weight>height)?weight:height;
				CRect dest;
				dlg->m_attach.GetClientRect(&dest);
				int destmax=(dest.Width()<dest.Height())?dest.Height():dest.Width();
				// 计算将图片缩放到TheImage区域所需的比例因子
				float scale = (float) ( (float) Sourcemax / (float)destmax );
    
				// 缩放后图片的宽和高
				int neww = (int)( weight/scale );
				int newh= (int)( height/scale );

				// 为了将缩放后的图片存入正中部位，需计算图片在 rectDraw 左上角的期望坐标值
				int tlx = (neww > newh)? 0: (int)(dest.Width()-neww)/2;
				int tly = (neww > newh)? (int)(dest.Height()-newh)/2: 0;

				// 设置 rectDraw,用来存入图片image
				CRect rect = dest;
				rect.SetRect(tlx, tly, neww, newh);

				image.Draw(dlg->m_attach.GetDC()->GetSafeHdc(), rect);
				
			}
		}

		char buff[100]="250 data\r\n";
		Send(buff,strlen(buff));//服务器响应250，表示请求的命令成功完成
		dlg->m_log.AddString(L"S:250  Message accepted for delivery!");
		data=false;
		AsyncSelect(FD_READ); //触发通信socket的OnReceive函数 
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
	// TODO: 在此添加专用代码和/或调用基类
	CSMTPServerDlg *dlg=(CSMTPServerDlg*)AfxGetApp()->GetMainWnd();//主窗口指针对象
	char buffer[100]= "220 ready\r\n";
	Send(buffer,strlen(buffer));//响应220，连接
	dlg->m_log.AddString(L"S:220 Ready For Mail Service");
	AsyncSelect(FD_READ);//触发OnReceive，接收命令并处理
	CAsyncSocket::OnSend(nErrorCode);
}

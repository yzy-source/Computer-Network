
// SMTPServerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SMTPServer.h"
#include "SMTPServerDlg.h"
#include "afxdialogex.h"
#include "ListenServer.h"
#include "ServerSocket.h"

#include<string>
#include <regex>
#include <sstream>
#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSMTPServerDlg �Ի���



CSMTPServerDlg::CSMTPServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSMTPServerDlg::IDD, pParent)
	, m_name(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSMTPServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TEXT, m_text);
	DDX_Control(pDX, IDC_LOG, m_log);
	DDX_Control(pDX, IDC_MESSAGE, m_message);
	DDX_Control(pDX, IDC_PICTURE, m_attach);
	DDX_Text(pDX, IDC_NAME, m_name);
}

BEGIN_MESSAGE_MAP(CSMTPServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CSMTPServerDlg ��Ϣ�������

BOOL CSMTPServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	m_log.ResetContent();  //ԭ���������
	m_log.AddString(L"SMTP��ʼ��������\n");
	m_log.AddString(L"���ڼ�������\n");
	m_message.SetHorizontalExtent(1000);//����ˮƽ������
	//m_log.SetHorizontalExtent();

	if(listen.m_hSocket ==INVALID_SOCKET)  
    {  
		
        //���������׽���,����FD_ACCEPT�¼�,Ĭ�϶˿�25
        BOOL bFlag =listen.Create(25,SOCK_STREAM,FD_ACCEPT);    //������������ʾm_srvrSocketֻ��FD_ACCEPT�¼�����Ȥ  
		
		if(!bFlag)  
        {  
            AfxMessageBox(_T("Socket����ʧ��!"));  
            listen.Close();  
            PostQuitMessage(0);  
  
            return TRUE;   
        }   
        //�����ɹ�,��ʾ������׼���ã����ȴ���������  
        if(!listen.Listen())//�������ʧ��  
        {  
            int nErrorCode =listen.GetLastError();    //��������Ϣ  
            if(nErrorCode !=WSAEWOULDBLOCK)             //��������߳�����  
            {  
                AfxMessageBox(_T("Socket����!"));  
                listen.Close();  
                PostQuitMessage(0);  
                return TRUE;  
            }  
        }  
		
    }  

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CSMTPServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSMTPServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CSMTPServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



CString CSMTPServerDlg::Decodetext(CString inputstr, int * length)
{
		 unsigned char * base64=(unsigned char *)"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
	 int n,i,j,assign;  
     unsigned char *p;  
     static unsigned char *string;  //��Ž������ַ���
     unsigned char * src;       //��Ž��յ���base64�ַ���
     *length=0;                 //��Ž���֮���ASCII��
     assign=0;     
     n=inputstr.GetLength();    //����ַ������Ƚ�������src��
     src=new unsigned char [n];  
     for(i=0;i<n;i++)  
         src[i]=inputstr[i];    //�������ַ����Ž�src��
   
     while(n>0&&src[n-1]=='=') {   //�ж������ַ���ĩβ��û��"="-�����������ֽ�
         src[n-1]=0;  
         assign++;  //�м����Ⱥ�
         n--;  
     }  
     for(i=0;i<n;i++)   { //base 64 ASCII��ӳ�䵽 6 bit  
         p=(unsigned char *)strchr((const char *)base64,(int)src[i]);  //(int)src[i]֮����ַ�����p��ʼ
         if(!p)  
              break;  
         src[i]=p-(unsigned char *)base64;  //ӳ�䵽6bit�ַ�
     }  
   
     string=(unsigned char *)malloc(n*3/4+1);  //����8bit���͵�str�ռ䣬��ʵ��n*6/8,\0ռһλ
     memset(string,0,n*3/4+1); //��ʼ��string 
     for(i=0,j=0;i<n;i+=4,j+=3) {  
         string[j]=(src[i]<<2) + ((src[i+1]&0x30)>>4);  //��һ��6bit��ǰ��λ+�ڶ���6bit��ǰ��λ
         string[j+1]=((src[i+1]&0x0F)<<4) + ((src[i+2]&0x3C)>>2);  //�ڶ���6bit�ĺ���λ+������6bit��ǰ��λ
         string[j+2]=((src[i+2]&0x03)<<6) + src[i+3];  //������6bit�ĺ���λ+���ĸ�6bit
         *length+=3;  //����һ���ֽ���Ŀ��3
     }  
     *length-=assign;   //�м����Ⱥż�ȥ�������ֽ�
     return CString(string);
}


void CSMTPServerDlg::DecodePicture(CString input, std::vector<char>& bytes)
{
	unsigned char * base64=(unsigned char *)"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

	 int n,i,j,assign;
     unsigned char *p;
     
     assign=0;
     n=input.GetLength();  //�ַ�������

     CString src;
	 src.GetBufferSetLength(n);   //���볤��Ϊn�Ŀռ�
     for(i=0;i<n;i++)
         src.SetAt(i,input[i]);   //��input���Ƶ�src
 
     while(n>0&&src[n-1]=='=')    //�ж������ַ���ĩβ��û��"="-�����������ֽ�
	 {
		 src.SetAt(n-1,0);
         assign++;               //"="�ĸ���
         n--;
     }
     for(i=0;i<n;i++)   
	 {                           //base 64 ASCII��ӳ�䵽 6 bit 
         p=(unsigned char *)strchr((const char *)base64,(int)src[i]);//p���ض�Ӧbase64�ַ���λ���Ӵ�
         if(!p)
              break;
		 src.SetAt(i,p-(unsigned char *)base64); //ӳ�䵽6 bit
     }
 
     for(i=0;i<n;i+=4) 
	 {
		 byte Byte[3];
         Byte[0]=(src[i]<<2)+((src[i+1]&0x30)>>4);
         Byte[1]=((src[i+1]&0x0F)<<4)+((src[i+2]&0x3C)>>2);
         Byte[2]=((src[i+2]&0x03)<<6)+src[i+3];

		 for(j=0;j<3;j++) 
			bytes.push_back(Byte[j]);   //8 bit ���뵽Byte��
     }

}

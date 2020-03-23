
// SMTPServerDlg.cpp : 实现文件
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


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CSMTPServerDlg 对话框



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


// CSMTPServerDlg 消息处理程序

BOOL CSMTPServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	m_log.ResetContent();  //原有数据清除
	m_log.AddString(L"SMTP开始工作……\n");
	m_log.AddString(L"正在监听……\n");
	m_message.SetHorizontalExtent(1000);//设置水平滚动条
	//m_log.SetHorizontalExtent();

	if(listen.m_hSocket ==INVALID_SOCKET)  
    {  
		
        //创建监听套接字,激发FD_ACCEPT事件,默认端口25
        BOOL bFlag =listen.Create(25,SOCK_STREAM,FD_ACCEPT);    //第三个参数表示m_srvrSocket只对FD_ACCEPT事件感兴趣  
		
		if(!bFlag)  
        {  
            AfxMessageBox(_T("Socket创建失败!"));  
            listen.Close();  
            PostQuitMessage(0);  
  
            return TRUE;   
        }   
        //监听成功,显示服务器准备好，并等待连接请求  
        if(!listen.Listen())//如果监听失败  
        {  
            int nErrorCode =listen.GetLastError();    //检测错误信息  
            if(nErrorCode !=WSAEWOULDBLOCK)             //如果不是线程阻塞  
            {  
                AfxMessageBox(_T("Socket错误!"));  
                listen.Close();  
                PostQuitMessage(0);  
                return TRUE;  
            }  
        }  
		
    }  

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSMTPServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSMTPServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



CString CSMTPServerDlg::Decodetext(CString inputstr, int * length)
{
		 unsigned char * base64=(unsigned char *)"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
	 int n,i,j,assign;  
     unsigned char *p;  
     static unsigned char *string;  //存放解码后的字符串
     unsigned char * src;       //存放接收到的base64字符串
     *length=0;                 //存放解码之后的ASCII码
     assign=0;     
     n=inputstr.GetLength();    //获得字符串长度建立数组src中
     src=new unsigned char [n];  
     for(i=0;i<n;i++)  
         src[i]=inputstr[i];    //把输入字符串放进src中
   
     while(n>0&&src[n-1]=='=') {   //判断输入字符串末尾有没有"="-用来填充的零字节
         src[n-1]=0;  
         assign++;  //有几个等号
         n--;  
     }  
     for(i=0;i<n;i++)   { //base 64 ASCII码映射到 6 bit  
         p=(unsigned char *)strchr((const char *)base64,(int)src[i]);  //(int)src[i]之后的字符串从p起始
         if(!p)  
              break;  
         src[i]=p-(unsigned char *)base64;  //映射到6bit字符
     }  
   
     string=(unsigned char *)malloc(n*3/4+1);  //分配8bit类型的str空间，其实是n*6/8,\0占一位
     memset(string,0,n*3/4+1); //初始化string 
     for(i=0,j=0;i<n;i+=4,j+=3) {  
         string[j]=(src[i]<<2) + ((src[i+1]&0x30)>>4);  //第一个6bit的前四位+第二个6bit的前两位
         string[j+1]=((src[i+1]&0x0F)<<4) + ((src[i+2]&0x3C)>>2);  //第二个6bit的后四位+第三个6bit的前四位
         string[j+2]=((src[i+2]&0x03)<<6) + src[i+3];  //第三个6bit的后两位+第四个6bit
         *length+=3;  //分析一组字节数目加3
     }  
     *length-=assign;   //有几个等号减去几个空字节
     return CString(string);
}


void CSMTPServerDlg::DecodePicture(CString input, std::vector<char>& bytes)
{
	unsigned char * base64=(unsigned char *)"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

	 int n,i,j,assign;
     unsigned char *p;
     
     assign=0;
     n=input.GetLength();  //字符串长度

     CString src;
	 src.GetBufferSetLength(n);   //申请长度为n的空间
     for(i=0;i<n;i++)
         src.SetAt(i,input[i]);   //把input复制到src
 
     while(n>0&&src[n-1]=='=')    //判断输入字符串末尾有没有"="-用来填充的零字节
	 {
		 src.SetAt(n-1,0);
         assign++;               //"="的个数
         n--;
     }
     for(i=0;i<n;i++)   
	 {                           //base 64 ASCII码映射到 6 bit 
         p=(unsigned char *)strchr((const char *)base64,(int)src[i]);//p返回对应base64字符的位置子串
         if(!p)
              break;
		 src.SetAt(i,p-(unsigned char *)base64); //映射到6 bit
     }
 
     for(i=0;i<n;i+=4) 
	 {
		 byte Byte[3];
         Byte[0]=(src[i]<<2)+((src[i+1]&0x30)>>4);
         Byte[1]=((src[i+1]&0x0F)<<4)+((src[i+2]&0x3C)>>2);
         Byte[2]=((src[i+2]&0x03)<<6)+src[i+3];

		 for(j=0;j<3;j++) 
			bytes.push_back(Byte[j]);   //8 bit 放入到Byte中
     }

}

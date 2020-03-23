
// SMTPServerDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "listenserver.h"

#include<string>
#include <regex>
#include <sstream>
#include <fstream>

// CSMTPServerDlg 对话框
class CSMTPServerDlg : public CDialogEx
{
// 构造
public:
	CSMTPServerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SMTPSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListBox m_text;
	CListBox m_log;
	CListBox m_message;
	CStatic m_attach;
	ListenServer listen;
	CString Decodetext(CString inputstr, int * length);
	void DecodePicture(CString input, std::vector<char> &bytes);
	CString m_name;
};

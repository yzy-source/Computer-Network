
// SMTPServerDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "listenserver.h"

#include<string>
#include <regex>
#include <sstream>
#include <fstream>

// CSMTPServerDlg �Ի���
class CSMTPServerDlg : public CDialogEx
{
// ����
public:
	CSMTPServerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SMTPSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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

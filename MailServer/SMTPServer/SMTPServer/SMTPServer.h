
// SMTPServer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSMTPServerApp:
// �йش����ʵ�֣������ SMTPServer.cpp
//

class CSMTPServerApp : public CWinApp
{
public:
	CSMTPServerApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSMTPServerApp theApp;
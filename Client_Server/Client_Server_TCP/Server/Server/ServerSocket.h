#pragma once

// ServerSocket ����Ŀ��

class ServerSocket : public CAsyncSocket
{
public:
	ServerSocket();
	virtual ~ServerSocket();
	virtual void OnAccept(int nErrorCode);
	CString IP;
	UINT port;
	TCHAR request[100];
	virtual void OnReceive(int nErrorCode);
	
};



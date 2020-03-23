#pragma once

// ServerSocket ÃüÁîÄ¿±ê

class ServerSocket : public CAsyncSocket
{
public:
	ServerSocket();
	virtual ~ServerSocket();
	virtual void OnReceive(int nErrorCode);
	CString IP;
	UINT port;
	TCHAR request[100];
};



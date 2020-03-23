#pragma once

// ListenServer ÃüÁîÄ¿±ê

class ListenServer : public CAsyncSocket
{
public:
	ListenServer();
	virtual ~ListenServer();
	virtual void OnAccept(int nErrorCode);
};



#pragma once

// ListenServer ����Ŀ��

class ListenServer : public CAsyncSocket
{
public:
	ListenServer();
	virtual ~ListenServer();
	virtual void OnAccept(int nErrorCode);
};



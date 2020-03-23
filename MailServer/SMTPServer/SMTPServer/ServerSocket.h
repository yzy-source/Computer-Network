#pragma once

// ServerSocket 命令目标

class ServerSocket : public CAsyncSocket
{
public:
	ServerSocket();
	virtual ~ServerSocket();
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	bool end;//判断data是否结束
	bool data;//判断是命令还是数据
	UINT length;//请求长度
	CString revtext;//data内容
	char Buffer[98000];//接收请求缓冲区
};



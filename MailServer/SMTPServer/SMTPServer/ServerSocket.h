#pragma once

// ServerSocket ����Ŀ��

class ServerSocket : public CAsyncSocket
{
public:
	ServerSocket();
	virtual ~ServerSocket();
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	bool end;//�ж�data�Ƿ����
	bool data;//�ж������������
	UINT length;//���󳤶�
	CString revtext;//data����
	char Buffer[98000];//�������󻺳���
};



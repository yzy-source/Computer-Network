// *******************************************************************
// client.cpp : Defines the entry point for the console application.
// *******************************************************************
//#include "stdafx.h"
#include <winsock2.h>
#include <iostream>
#define PORT_SERVER 6656

int main(int argc, char* argv[])
{
 std::cout << "Start up tcp client." << std::endl;
 
 WSADATA wsaData;
 WORD sockVersion = MAKEWORD(2, 0);
 if (0 != WSAStartup(sockVersion, &wsaData))
 {
  std::cout << "Failed to retrive socket version." 
   << std::endl;
  return 0;
 }
 
 SOCKET sock_client;
 sock_client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
 if (INVALID_SOCKET == sock_client)
 {
  std::cout << "Invalid socket." << std::endl;
  WSACleanup();
  return 0;
 }
 sockaddr_in addr_sev;
 addr_sev.sin_family = AF_INET;
 addr_sev.sin_port = htons(PORT_SERVER);
 addr_sev.sin_addr.s_addr = inet_addr("127.0.0.1");
 if (SOCKET_ERROR == connect(sock_client, (sockaddr *)&addr_sev, sizeof(addr_sev)))
 {
  std::cout << "Failed to connect." << std::endl;
  WSACleanup();
  return 0;
 }
 char buf[512];
 int recv_size = 0;
 recv_size = recv(sock_client, buf, 512, 0);
 if (512 > recv_size)
 {
  buf[recv_size] = '\0';
  std::cout << "receive data is :" << buf << std::endl;
  memset(buf, 0, sizeof(buf));
 }
 else
 {
  std::cout << "receive data is overflow" << std::endl;
 }
 
 closesocket(sock_client);
 WSACleanup();
 system("PAUSE");
 return 0;
}
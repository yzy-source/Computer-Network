
// *******************************************************************
// server.cpp : Defines the entry point for the console application.
// *******************************************************************
//#include "stdafx.h"
#include <winsock2.h>
#include <iostream>
#define PORT_SERVER 6656
#define NUM_CLIENTS 10
int main(int argc, char* argv[])
{
 std::cout << "Start up tcp server." << std::endl;
 
 WSADATA wsaData;
 WORD sockVersion = MAKEWORD(2, 0);
 if (0 != WSAStartup(sockVersion, &wsaData))
 {
  std::cout << "Failed to retrive socket version." 
         << std::endl;
  return 0;
 } 
 SOCKET sock_sev;
 sock_sev = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
 if (INVALID_SOCKET == sock_sev)
 {
  std::cout << "Invalid socket." << std::endl;
  WSACleanup();
  return 0;
 }
 sockaddr_in addr_sev;
 addr_sev.sin_family = AF_INET;
 addr_sev.sin_port = htons(PORT_SERVER);
 addr_sev.sin_addr.s_addr = INADDR_ANY;
 if (SOCKET_ERROR == bind(sock_sev, (sockaddr *)&addr_sev, sizeof(addr_sev)))
 {
  std::cout << "Failed to bind." << std::endl;
  WSACleanup();
  return 0;
 }
 if (SOCKET_ERROR == listen(sock_sev, NUM_CLIENTS))
 {
  std::cout << "Failed to listen." << std::endl;
  WSACleanup();
  return 0;
 }
 
 // 多路复用,暂时还未实现.后期会对此给出示例.
 //fd_set reads;
 //fd_set writes;
 //fd_set accepts;
 //FD_ZERO(&reads);
 //FD_ZERO(&writes);
 //FD_ZERO(&accepts);
 //SOCKET sock_client;

 sockaddr_in addr_client;
 int nAddrLen = sizeof(addr_client);
 while (true)
 {
  char tmp[512];
  sock_client = accept(sock_sev, (sockaddr *)&addr_client, &nAddrLen);
   
  if (INVALID_SOCKET == sock_client)
  {
   std::cout << "Failed to accept." << std::endl;
   continue;
  }
  sprintf(tmp, "Your IP is %s\n", inet_ntoa(addr_client.sin_addr));
  send(sock_client, tmp, strlen(tmp), 0);
  std::cout << "Connection from " << inet_ntoa(addr_client.sin_addr)
         << std::endl;
  closesocket(sock_client);
 }
 
 return 0;
}
 
//注:本程序使用的是VC6 IDE,编译时请在link中加入对ws2_32.lib库的引用.

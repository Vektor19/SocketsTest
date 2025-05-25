#pragma once
#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include "SocketHandle.h"
#include "EIpVersion.h"
#include "ESocketOption.h"
#include "EResult.h"
#include "IpEndpoint.h"
#include <string>
namespace networking {
	class TcpSocket;
	class HttpServer
	{
	public:
		HttpServer(std::string& executablePath);
		~HttpServer();
		EResult start(int port);
	private:
		TcpSocket m_tcpSocket;
		std::string m_rootPath;
		EResult handleClient(TcpSocket acceptSocket);
	};

}
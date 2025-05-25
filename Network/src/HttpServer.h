#pragma once
#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include "SocketHandle.h"
#include "EIpVersion.h"
#include "ESocketOption.h"
#include "EResult.h"
#include "IpEndpoint.h"
namespace networking {
	class TcpSocket;
	class HttpServer
	{
	public:
		HttpServer();
		~HttpServer();
		EResult start(int port);
		EResult handleClient(const TcpSocket& acceptSocket);
	private:
		TcpSocket m_tcpSocket;
	};

}
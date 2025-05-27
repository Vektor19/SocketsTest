#pragma once
#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include "SocketHandle.h"
#include "EIpVersion.h"
#include "ESocketOption.h"
#include "EResult.h"
#include "IpEndpoint.h"
#include "TcpSocket.h"
#include <string>
#include <mutex>
namespace networking {
	class Server
	{
	public:
		Server(std::string& executablePath);
		~Server();
		virtual EResult start(int port) = 0;
	protected:
		TcpSocket m_tcpSocket;
		std::string m_rootPath;
		std::string m_resPath;
		virtual EResult handleClient(TcpSocket acceptSocket) = 0;
		std::string getContentType(const std::string& path);
		static std::mutex mtx;
	};

}
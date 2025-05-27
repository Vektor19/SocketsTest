#pragma once
#include "Server.h"
namespace networking {
	class TcpSocket;
	class HttpsServer : public Server
	{
	public:
		HttpsServer(std::string& executablePath);
		~HttpsServer();
		EResult start(int port) override;
	private:
		EResult handleClient(TcpSocket acceptSocket) override;
	};
}
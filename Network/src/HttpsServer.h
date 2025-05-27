#pragma once
#include "Server.h"
#include <openssl/ssl.h>

namespace networking {
	class TcpSocket;
	class HttpsServer : public Server
	{
	public:
		HttpsServer(std::string& executablePath);
		~HttpsServer();
		EResult start(int port) override;
	private:
		SSL_CTX* m_pSslCtx = nullptr;
		EResult handleClient(TcpSocket acceptSocket) override;
	};
}
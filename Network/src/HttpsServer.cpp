#include "TcpSocket.h"
#include <assert.h>
#include "HttpsServer.h"
#include "Server.h"
#include <thread>
#include <iostream>
#include "Request.h"
#include "Response.h"
#include <IOUtils.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <exception>

namespace networking {
	HttpsServer::HttpsServer(std::string& executablePath) : Server(executablePath)
	{
		std::string certPath = m_rootPath + "certs\\cert.pem";
		std::string keyPath = m_rootPath + "certs\\key.pem";

		const SSL_METHOD* method = TLS_server_method();
		m_pSslCtx = SSL_CTX_new(method);
		if (!m_pSslCtx) {
			ERR_print_errors_fp(stderr);
			throw std::exception("Can't start initialize SSL context");
		}

		if (SSL_CTX_use_certificate_file(m_pSslCtx, certPath.c_str(), SSL_FILETYPE_PEM) <= 0 ||
			SSL_CTX_use_PrivateKey_file(m_pSslCtx, keyPath.c_str(), SSL_FILETYPE_PEM) <= 0 ||
			!SSL_CTX_check_private_key(m_pSslCtx)) {
			ERR_print_errors_fp(stderr);
			SSL_CTX_free(m_pSslCtx);
			throw std::exception("Can't process certificate or private key");
		}
	}
	HttpsServer::~HttpsServer()
	{
		if (m_pSslCtx) {
			SSL_CTX_free(m_pSslCtx);
			m_pSslCtx = nullptr;
		}
	}

	EResult HttpsServer::start(int port)
	{
		if (m_tcpSocket.listen(IpEndpoint("192.168.0.108", port), 5) == EResult::Success)
		{
			std::cout << "Socket successfuly listening on port " << port << std::endl;
			while (true)
			{
				TcpSocket connectionSocket;
				IpEndpoint newConnectionEndpoint;
				if (m_tcpSocket.accept(connectionSocket, newConnectionEndpoint) == EResult::Success)
				{
					{
						std::lock_guard<std::mutex> guard(mtx);
						std::cout << "Accepted new conneciton from: " << std::endl;
						newConnectionEndpoint.print();
					}
					std::thread handlingThread(&HttpsServer::handleClient, this, std::move(connectionSocket));

					handlingThread.detach();
				}
				else
				{
					std::cerr << "Failed to accept connection." << std::endl;
				}
			}
		}
		else
		{
			std::cerr << "Failed to listen on port " << port << std::endl;
			return EResult::NotYetImplemented;
		}

		return EResult::Success;
	}

	EResult HttpsServer::handleClient(TcpSocket acceptSocket)
	{
		const int bufferSize = 1024;

		SSL* ssl = SSL_new(m_pSslCtx);
		SSL_set_fd(ssl, acceptSocket.getSocketHandle());

		if (SSL_accept(ssl) <= 0) {
			ERR_print_errors_fp(stderr);
			SSL_free(ssl);
			return EResult::NotYetImplemented;
		}

		int bytesRead = 0;
		std::string requestStr = "";
		do
		{
			char buffer[bufferSize];

			bytesRead = SSL_read(ssl, buffer, bufferSize);
			requestStr.append(buffer, bytesRead);
		} while (bytesRead == bufferSize);
		{
			std::lock_guard<std::mutex> guard();
			std::cout << requestStr << std::endl;
		}
		Request request;
		Response response;
		switch (request.parseFromString(requestStr))
		{
		case ParseResult::BadMethod:
			response.setStatus(ResponseStatus::MethodNotAllowed);
			break;
		case ParseResult::BadRequest:
			response.setStatus(ResponseStatus::BadRequest);
			break;
		case ParseResult::OK: {
			const std::string& method = request.getMethod();
			if (method == "GET")
			{
				if (request.getUriPath() == "/")
				{
					response.setBody(IOUtils::getFileString(m_resPath + "index.html"));
					response.addHeader("Content-Type", "text/html");
				}
				else
				{
					std::string body = IOUtils::getFileString(m_resPath + request.getUriPath().substr(1));
					if (body == "")
					{
						response.setBody(IOUtils::getFileString(m_resPath + "404.html"));
						response.addHeader("Content-Type", "text/html");
						response.setStatus(ResponseStatus::NotFound);
					}
					else
					{
						response.setBody(body);
						response.addHeader("Content-Type", getContentType(request.getUriPath()));
						response.setStatus(ResponseStatus::OK);
					}
				}
			}
			break;
		}
		default:
			break;
		}
		std::string responseStr = response.toString();
		SSL_write(ssl, responseStr.c_str(), static_cast<int>(responseStr.size()));

		SSL_shutdown(ssl);
		SSL_free(ssl);

		acceptSocket.shutdown(EShutdownType::Both);
		acceptSocket.close();
		return EResult::Success;
	}
}
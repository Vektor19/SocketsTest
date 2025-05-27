#include "TcpSocket.h"
#include <assert.h>
#include "HttpServer.h"
#include <thread>
#include <iostream>
#include "Request.h"
#include "Response.h"
#include <IOUtils.h>

namespace networking {
	HttpServer::HttpServer(std::string& executablePath): Server(executablePath)
	{
	}
	HttpServer::~HttpServer()
	{
	}

	EResult HttpServer::start(int port)
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
					std::thread handlingThread(&HttpServer::handleClient, this, std::move(connectionSocket));

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
			std::cerr << "Failed to listen on port "<< port << std::endl;
			return EResult::NotYetImplemented;
		}

		return EResult::Success;
	}

	EResult HttpServer::handleClient(TcpSocket acceptSocket)
	{
		int byteReceived = 0;
		const int bufferSize = 1024;
		std::string requestStr = "";
		do
		{
			char buffer[bufferSize];
			if (acceptSocket.recv(buffer, bufferSize, byteReceived) != EResult::Success)
			{
				std::cerr << "Failed to receive data from request." << std::endl;
				return EResult::NotYetImplemented;
			}
			requestStr.append(buffer, byteReceived);
		} while (byteReceived == bufferSize);
		{
			std::lock_guard<std::mutex> guard(mtx);
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
		if (acceptSocket.sendAll(&responseStr[0], responseStr.size()) != EResult::Success)
		{
			std::cerr << "Failed to send response" << std::endl;
			return EResult::NotYetImplemented;
		}

		acceptSocket.shutdown(EShutdownType::Both);
		acceptSocket.close();

		return EResult::Success;
	}
}
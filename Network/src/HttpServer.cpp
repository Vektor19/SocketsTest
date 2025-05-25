#include "TcpSocket.h"
#include <assert.h>
#include "HttpServer.h"
#include <thread>
#include <mutex>
#include <iostream>
#include "Request.h"
#include "Response.h"

namespace networking {
	std::mutex mtx;
	HttpServer::HttpServer(std::string& executablePath)
	{
		if (m_tcpSocket.create() == EResult::Success)
		{
			std::cout << "Socket created successfuly" << std::endl;
		}
		else
		{
			std::cerr << "Couldn't create socket." << std::endl;
		}

		size_t found = executablePath.find_last_of("/\\");
		m_rootPath = executablePath.substr(0, found + 1)+"res\\";
	}
	HttpServer::~HttpServer()
	{
		m_tcpSocket.close();
	}

	EResult HttpServer::start(int port)
	{
		if (m_tcpSocket.listen(IpEndpoint("192.168.5.102", port), 5) == EResult::Success)
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
			std::cerr << "Failed to listen on 5555 port." << std::endl;
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
			
			break;
		case ParseResult::BadRequest:
			break;
		case ParseResult::OK:
			break;
		default:
			break;
		}
		


		acceptSocket.shutdown(EShutdownType::Both);
		acceptSocket.close();

		return EResult::Success;
	}

}
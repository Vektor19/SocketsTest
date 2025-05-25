#include "TcpSocket.h"
#include <assert.h>
#include "HttpServer.h"
#include <thread>
#include <mutex>
#include <iostream>
#include "Request.h"
#include "Response.h"
#include <IOUtils.h>

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
		m_rootPath = executablePath.substr(0, found + 1) + "res\\";
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
					response.setBody(IOUtils::getFileString(m_rootPath + "index.html"));
					response.addHeader("Content-Type", "text/html");
				}
				else
				{
					std::string body = IOUtils::getFileString(m_rootPath + request.getUriPath().substr(1));
					if (body == "")
					{
						response.setBody(IOUtils::getFileString(m_rootPath + "404.html"));
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

	std::string HttpServer::getContentType(const std::string& path)
	{
		auto endsWith = [](const std::string& str, const std::string& suffix) {
			if (str.length() < suffix.length()) return false;
			return str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
			};

		if (endsWith(path, ".html")) return "text/html";
		if (endsWith(path, ".css"))  return "text/css";
		if (endsWith(path, ".js"))   return "application/javascript";
		if (endsWith(path, ".json")) return "application/json";
		if (endsWith(path, ".png"))  return "image/png";
		if (endsWith(path, ".jpg") || endsWith(path, ".jpeg")) return "image/jpeg";
		if (endsWith(path, ".gif"))  return "image/gif";
		if (endsWith(path, ".svg"))  return "image/svg+xml";
		if (endsWith(path, ".ico"))  return "image/x-icon";

		return "*/*";
	}


}
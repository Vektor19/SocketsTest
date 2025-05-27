#include <assert.h>
#include "Server.h"
#include <thread>
#include <mutex>
#include <iostream>
#include "Request.h"
#include "Response.h"
#include <IOUtils.h>

namespace networking {

	std::mutex Server::mtx;
	Server::Server(std::string& executablePath)
	{
		m_tcpSocket = TcpSocket(EIpVersion::IPv4);
		if (m_tcpSocket.create() == EResult::Success)
		{
			std::cout << "Socket created successfuly" << std::endl;
		}
		else
		{
			std::cerr << "Couldn't create socket." << std::endl;
		}

		size_t found = executablePath.find_last_of("/\\");
		m_rootPath = executablePath.substr(0, found + 1);
		m_resPath = m_rootPath + "res\\";
	}
	Server::~Server()
	{
		m_tcpSocket.close();
	}

	std::string Server::getContentType(const std::string& path)
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
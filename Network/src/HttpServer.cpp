#include "TcpSocket.h"
#include <assert.h>
#include "HttpServer.h"
#include <iostream>
namespace networking {
	HttpServer::HttpServer()
	{
		if (m_tcpSocket.create() == EResult::Success)
		{
			std::cout << "Socket created successfuly" << std::endl;
		}
		else
		{
			std::cerr << "Couldn't create socket." << std::endl;
		}
	}
	HttpServer::~HttpServer()
	{
	}

	EResult HttpServer::start(int port)
	{

		return EResult();
	}

	EResult HttpServer::handleClient(const TcpSocket& acceptSocket)
	{
		return EResult();
	}

}
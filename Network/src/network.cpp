#include "network.h"
#include <iostream>
#include <openssl/ssl.h>
#include <openssl/err.h>
namespace networking
{
	bool Network::initialize()
	{
		WSADATA wsadata;
		int result = WSAStartup(MAKEWORD(2, 2), &wsadata);
		if (result != 0)
		{
			std::cerr << "Failed to start up the winsock API." << std::endl;
			return false;
		}

		if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wVersion) != 2)
		{
			std::cerr << "Couldn't find a usable version of the winsock api dll." << std::endl;
			return false;
		}

		SSL_library_init();
		SSL_load_error_strings();
		OpenSSL_add_all_algorithms();

		std::cout << "OpenSSL successfully initialized." << std::endl;
		return true;
	}

	void Network::shutdown()
	{
		WSACleanup();
	}
}

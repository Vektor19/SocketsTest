#include "network.h"
#include <iostream>

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

		return true;
    }

    void Network::shutdown()
    {
        WSACleanup();
    }
}

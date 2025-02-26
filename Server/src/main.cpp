#include <iostream>
#include "Network.h"

using namespace networking;

int main(int argc, char** argv)
{
	if (Network::initialize())
	{
		std::cout << "Winsock api successfully initialized." << std::endl;
		TcpSocket tcpSocket;
		if (tcpSocket.create() == EResult::Success)
		{
			std::cout << "Socket created successfuly" << std::endl;
			if (tcpSocket.bind(IpEndpoint("0.0.0.0", 5555)) == EResult::Success)
			{
				std::cout << "Socket bound successfuly" << std::endl;
			}
			else
			{
				std::cerr << "Failed to bind socket on 5555 port." << std::endl;
			}
			tcpSocket.close();
		}
		else
		{
			std::cerr << "Couldn't create socket." << std::endl;
		}
	}
	Network::shutdown();
	system("pause");
    return 0;
}
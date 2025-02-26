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
			if (tcpSocket.listen(IpEndpoint("0.0.0.0", 5555), 5) == EResult::Success)
			{
				std::cout << "Socket successfuly listening on port 5555" << std::endl;
				TcpSocket connectionSocket;
				IpEndpoint newConnectionEndpoint;
				if (tcpSocket.accept(connectionSocket, newConnectionEndpoint) == EResult::Success)
				{
					std::cout << "Accepted new conneciton." << std::endl;
					newConnectionEndpoint.print();
					connectionSocket.close();
				}
				else
				{
					std::cerr << "Failed to accept connection." << std::endl;
				}
			}
			else
			{
				std::cerr << "Failed to listen on 5555 port." << std::endl;
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
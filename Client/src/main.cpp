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
			if (tcpSocket.connect(IpEndpoint("127.0.0.1", 5555)) == EResult::Success)
			{
				std::cout << "Connected to the server." << std::endl;
				char buffer[1024];
				int bytesSent = 0;
				EResult result = EResult::Success;
				while (result == EResult::Success)
				{
					std::cin.getline(buffer, 1024);
					result = tcpSocket.send(buffer, 1024, bytesSent);
					Sleep(500);
					int bytesReceived = 0;
					result = tcpSocket.recv(buffer, 1024, bytesReceived);
					if (result != EResult::Success)
						break;
					std::cout << buffer << std::endl;
				}
			}
			else
			{
				std::cerr << "Failed to connect to the server." << std::endl;
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
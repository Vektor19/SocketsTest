#include <iostream>
#include "Network.h"

using namespace networking;

void runWithUdp();
void runWithTcp();

int main(int argc, char** argv)
{
	if (Network::initialize())
	{
		std::cout << "Winsock api successfully initialized." << std::endl;
		runWithUdp();
		//runWithTcp();
	}
	Network::shutdown();
	system("pause");
	return 0;
}

void runWithUdp()
{
	UdpSocket udpSocket;
	if (udpSocket.create() == EResult::Success)
	{
		std::cout << "Socket created successfuly" << std::endl;
		if (udpSocket.bind(IpEndpoint("0.0.0.0", 7777)) == EResult::Success)
		{
			std::cout << "Socket successfuly bound to port 7777" << std::endl;

			IpEndpoint serverEndpoint("127.0.0.1", 6666);
			char buffer[1024];
			int bytesSent = 0;
			while (true)
			{
				std::cin.getline(buffer, 1024);
				int bytesSent = 0;
				if (udpSocket.sendTo(serverEndpoint, buffer, 1024, bytesSent) != EResult::Success)
					break;

				int bytesReceived = 0;
				IpEndpoint incomingEndpoint;
				if (udpSocket.recvFrom(incomingEndpoint, buffer, 1024, bytesReceived))
					break;

				/*std::cout << "Sender Endpoint: " << std::endl;
				incomingEndpoint.print();*/
				std::cout << buffer << std::endl;

				
				Sleep(500);
			}

		}
		else
		{
			std::cerr << "Failed to bind to 7777 port." << std::endl;
		}
		udpSocket.close();
	}
	else
	{
		std::cerr << "Couldn't create socket." << std::endl;
	}
}
void runWithTcp()
{
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
#include <iostream>
#include "Network.h"
#include "Utils.h"

using namespace networking;

void runWithUdp();
void runWithTcp();
void benchmark();

int main(int argc, char** argv)
{
	if (Network::initialize())
	{
		std::cout << "Winsock api successfully initialized." << std::endl;
		//runWithUdp();
		//runWithTcp();
		benchmark();
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

			IpEndpoint serverEndpoint("192.168.5.102", 6666);
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
			EResult result = EResult::Success;
			while (result == EResult::Success)
			{
				std::cin.getline(buffer, 1024);
				if (strcmp(buffer, "exit") == 0)
				{
					break;
				}
				result = tcpSocket.sendAll(buffer, 1024);
				Sleep(500);
				int bytesReceived = 0;
				result = tcpSocket.recvAll(buffer, 1024);
				if (result != EResult::Success)
					break;
				std::cout << buffer << std::endl;
			}
		}
		else
		{
			std::cerr << "Failed to connect to the server." << std::endl;
		}
		tcpSocket.shutdown(EShutdownType::Both);
		tcpSocket.close();
	}
	else
	{
		std::cerr << "Couldn't create socket." << std::endl;
	}
}


void benchmark()
{
	TcpSocket tcpSocket;
	if (tcpSocket.create() == EResult::Success)
	{
		std::cout << "Socket created successfuly" << std::endl;
		if (tcpSocket.connect(IpEndpoint("192.168.0.108", 5555)) == EResult::Success)
		{
			std::cout << "Connected to the server." << std::endl;
			const int bufferSize = 8192;
			const int iterations = 1000000;
			char buffer[bufferSize];
			memset(buffer, 'a', 8192);
			EResult result = EResult::Success;
			Stopwatch stopwatch;
			stopwatch.start();
			for (size_t i = 0; i < iterations; i++)
			{
				result = tcpSocket.sendAll(buffer, bufferSize);
			}
			stopwatch.stop();
			if (result == EResult::Success)
			{
				std::cout << "Data sent!" << std::endl;
				double duration = stopwatch.getDurationInSeconds();
				double totalBytes = static_cast<double>(iterations) * bufferSize;
				double speed = static_cast<double>(totalBytes) / duration;
				std::cout << "Speed MB/s: " << (speed / 1024.0) / 1024.0 << std::endl;
				
			}
			else
			{
				std::cerr << "Error occured while sending!" << std::endl;
			}
			
		}
		else
		{
			std::cerr << "Failed to connect to the server." << std::endl;
		}
		tcpSocket.shutdown(EShutdownType::Both);
		tcpSocket.close();
	}
	else
	{
		std::cerr << "Couldn't create socket." << std::endl;
	}
}
#include <iostream>
#include "Network.h"
#include "Utils.h"

using namespace networking;

void runWithUdp();
void runWithTcp();
void benchmark();
void runHttpServer();

int main(int argc, char** argv)
{
	if (Network::initialize())
	{
		std::cout << "Winsock api successfully initialized." << std::endl;
		//runWithUdp();
		//runWithTcp();
		//benchmark();
		runHttpServer(argv[0]);
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
		if (udpSocket.bind(IpEndpoint("0.0.0.0", 6666)) == EResult::Success)
		{
			std::cout << "Socket successfuly bound to port 6666" << std::endl;

			IpEndpoint senderEndpoint;
			char buffer[1024];
			int bytesReceived = 0;
			while (true)
			{
				EResult result = udpSocket.recvFrom(senderEndpoint,buffer, 1024, bytesReceived);
				if (result != EResult::Success)
					break;

				/*std::cout << "Sender Endpoint: " << std::endl;
				senderEndpoint.print();*/
				std::cout << buffer << std::endl;

				std::cin.getline(buffer, 1024);
				int bytesSent = 0;
				if (udpSocket.sendTo(senderEndpoint, buffer, 1024, bytesSent) != EResult::Success)
					break;
				Sleep(500);
			}

		}
		else
		{
			std::cerr << "Failed to listen on 5555 port." << std::endl;
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
		if (tcpSocket.listen(IpEndpoint("0.0.0.0", 5555), 5) == EResult::Success)
		{
			std::cout << "Socket successfuly listening on port 5555" << std::endl;
			TcpSocket connectionSocket;
			IpEndpoint newConnectionEndpoint;
			if (tcpSocket.accept(connectionSocket, newConnectionEndpoint) == EResult::Success)
			{
				std::cout << "Accepted new conneciton." << std::endl;
				newConnectionEndpoint.print();

				char buffer[1024];
				while (true)
				{
					EResult result = connectionSocket.recvAll(buffer, 1024);
					if (result != EResult::Success)
						break;
					std::cout << buffer << std::endl;
					std::cin.getline(buffer, 1024);
					if (strcmp(buffer, "exit") == 0)
					{
						break;
					}
					int bytesSent = 0;
					if (connectionSocket.sendAll(buffer, 1024) != EResult::Success)
						break;
					Sleep(500);
				}
				connectionSocket.shutdown(EShutdownType::Both);
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

void benchmark()
{
	int bufferSize = 10000;
	std::cout << "Enter buffer size: ";
	std::cin >> bufferSize;
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
				const int iterations = 1000;
				char* buffer = new char[bufferSize];
				EResult result = EResult::Success;
				Stopwatch stopwatch;
				stopwatch.start();
				for (size_t i = 0; i < iterations; i++)
				{
					result = connectionSocket.recvAll(buffer, bufferSize);
				}
				stopwatch.stop();
				if (result == EResult::Success)
				{
					std::cout << "Data received!" << std::endl;
					double duration = stopwatch.getDurationInSeconds();
					double totalBytes = static_cast<double>(iterations) * bufferSize;
					double speed = static_cast<double>(totalBytes) / duration;
					std::cout << "Speed MB/s: " << (speed / 1024.0) / 1024.0 << std::endl;

				}
				else
				{
					std::cerr << "Error occured while receiving!" << std::endl;
				}
				delete[] buffer;
				connectionSocket.shutdown(EShutdownType::Both);
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

void runHttpServer(std::string& executablePath) 
{
	HttpServer server(executablePath);
	server.start(80);
}
#include <iostream>
#include "Network.h"

using namespace networking;

int main(int argc, char** argv)
{
	if (Network::initialize())
	{
		IpEndpoint endpoint("google.com", 5555);
		if (endpoint.getIpVersion() == EIpVersion::IPv4)
		{
			std::cout << "Hostname: " << endpoint .getHostname() << std::endl;
			std::cout << "Ip: " << endpoint.getIpStr() << std::endl;
			std::cout << "Port: " << endpoint.getPort() << std::endl;
			std::cout << "Ip Bytes: " << std::endl;
			for (auto &digit : endpoint.getIpBytes())
			{
				std::cout << (int)digit << std::endl;
			}
		}
		else
		{
			std::cout << "Not ipv4" << std::endl;
		}
		std::cout << "Winsock api successfully initialized." << std::endl;
		Socket socket;
		if (socket.create() == EResult::Success)
		{
			std::cout << "Socket created successfuly" << std::endl;
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
#include <iostream>
#include "Network.h"

using namespace networking;

int main(int argc, char** argv)
{
	if (Network::initialize())
	{
		std::cout << "Winsock api successfully initialized." << std::endl;
		Socket socket;
		if (socket.create() == EResult::Success)
		{
			std::cout << "Socket created successfuly" << std::endl;
			if (socket.bind(IpEndpoint("0.0.0.0", 5555)) == EResult::Success)
			{
				std::cout << "Socket bound successfuly" << std::endl;
			}
			else
			{
				std::cerr << "Failed to bind socket on 5555 port." << std::endl;
			}
			socket.close();
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
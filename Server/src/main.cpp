#include <iostream>
#include "network.h"

int main(int argc, char** argv)
{
	if (networking::Network::initialize())
	{
		std::cout << "Winsock api successfully initialized." << std::endl;
	}
	else
	{
		std::cerr << "Socket failed to create." << std::endl;
	}
	networking::Network::shutdown();
	system("pause");
    return 0;
}
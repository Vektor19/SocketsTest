#pragma once
#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include "Socket.h"
namespace networking {
	class Network
	{
	public:
		static bool initialize();
		static void shutdown();
	private:

	};

}
#pragma once
#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include "SocketHandle.h"
#include "EIpVersion.h"
#include "ESocketOption.h"
#include "EResult.h"
#include "IpEndpoint.h"
namespace networking {
	class Socket
	{
	public:
		Socket(EIpVersion ipVersion = EIpVersion::IPv4, SocketHandle socketHandle = INVALID_SOCKET);
		EResult create();
		EResult close();
		EIpVersion getIpVersion();
		SocketHandle getSocketHandle();
	private:
		EResult setSocketOption(ESocketOption socketOption, BOOL value);
		EIpVersion m_ipVersion = EIpVersion::IPv4;
		SocketHandle m_socketHandle = INVALID_SOCKET;
	};

}
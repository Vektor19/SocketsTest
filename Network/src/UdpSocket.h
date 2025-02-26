#pragma once
#include "Socket.h"
namespace networking {
	class UdpSocket: public Socket
	{
	public:
		UdpSocket(EIpVersion ipVersion = EIpVersion::IPv4, SocketHandle socketHandle = INVALID_SOCKET);
		EResult create() override;
	private:
		EResult setSocketOption(ESocketOption socketOption, BOOL value) override;
	};

}
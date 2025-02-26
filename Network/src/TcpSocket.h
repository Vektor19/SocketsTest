#pragma once
#include "Socket.h"
namespace networking {
	class TcpSocket: public Socket
	{
	public:
		TcpSocket(EIpVersion ipVersion = EIpVersion::IPv4, SocketHandle socketHandle = INVALID_SOCKET);
		EResult create() override;
	private:
		EResult setSocketOption(ESocketOption socketOption, BOOL value) override;
	};

}
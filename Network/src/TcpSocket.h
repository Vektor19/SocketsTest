#pragma once
#include "Socket.h"
namespace networking {
	class TcpSocket: public Socket
	{
	public:
		TcpSocket(EIpVersion ipVersion = EIpVersion::IPv4, SocketHandle socketHandle = INVALID_SOCKET);
		EResult create() override;
		EResult listen(IpEndpoint endpoint, int backlog);
		EResult accept(Socket& outSocket);
		EResult connect(IpEndpoint endpoint);
	private:
		EResult setSocketOption(ESocketOption socketOption, BOOL value) override;
	};

}
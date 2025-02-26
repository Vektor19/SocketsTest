#pragma once
#include "Socket.h"
namespace networking {
	class UdpSocket: public Socket
	{
	public:
		UdpSocket(EIpVersion ipVersion = EIpVersion::IPv4, SocketHandle socketHandle = INVALID_SOCKET);
		EResult create() override;
		EResult sendTo(IpEndpoint endpoint,const void* data, int numberOfBytes, int& bytesSent);
		EResult recvFrom(IpEndpoint& senderEndpoint, void* destination, int numberOfBytes, int& bytesReceived);
	private:
		EResult setSocketOption(ESocketOption socketOption, BOOL value) override;
	};

}
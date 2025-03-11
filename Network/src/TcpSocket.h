#pragma once
#include "Socket.h"
#include "EShutdownType.h"
namespace networking {
	class TcpSocket: public Socket
	{
	public:
		TcpSocket(EIpVersion ipVersion = EIpVersion::IPv4, SocketHandle socketHandle = INVALID_SOCKET);
		EResult create() override;
		EResult listen(IpEndpoint endpoint, int backlog);
		EResult accept(Socket& outSocket, IpEndpoint& outEndpoint);
		EResult connect(IpEndpoint endpoint);
		EResult send(const void* data, int numberOfBytes, int& bytesSent);
		EResult sendAll(const void* data, int numberOfBytes);
		EResult recv(void* destination, int numberOfBytes, int& bytesRecieved);
		EResult recvAll(const void* destination, int numberOfBytes);
		EResult shutdown(EShutdownType eShutdownType = EShutdownType::Both);
	private:
		EResult setSocketOption(ESocketOption socketOption, BOOL value) override;
	};

}
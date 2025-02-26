#include "UdpSocket.h"
#include <assert.h>
namespace networking {

	UdpSocket::UdpSocket(EIpVersion ipVersion, SocketHandle socketHandle)
		: Socket(ipVersion, socketHandle)
	{
		assert(m_ipVersion == EIpVersion::IPv4);
	}

	EResult UdpSocket::create()
	{
		assert(m_ipVersion == EIpVersion::IPv4);
		if (m_socketHandle != INVALID_SOCKET)
		{
			return EResult::NotYetImplemented;
		}
		m_socketHandle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (m_socketHandle==INVALID_SOCKET)
		{
			int error = WSAGetLastError();
			return EResult::NotYetImplemented;
		}
		return EResult::Success;
	}

	EResult UdpSocket::sendTo(IpEndpoint endpoint, const void* data, int numberOfBytes, int& bytesSent)
	{
		sockaddr_in addr = endpoint.getSockaddrIPv4();
		bytesSent = ::sendto(m_socketHandle, (const char*)data, numberOfBytes, NULL, (sockaddr*)(&addr), sizeof(sockaddr_in));
		if (bytesSent == SOCKET_ERROR)
		{
			int error = WSAGetLastError();
			return EResult::NotYetImplemented;
		}
		return EResult::Success;
	}

	EResult UdpSocket::recvFrom(IpEndpoint& senderEndpoint, void* destination, int numberOfBytes, int& bytesReceived)
	{
		sockaddr_in senderAddr = {};
		int fromlen = sizeof(senderAddr);

		bytesReceived = ::recvfrom(m_socketHandle, (char*)destination, numberOfBytes, 0, (sockaddr*)&senderAddr, &fromlen);

		if (bytesReceived == 0)
		{
			return EResult::NotYetImplemented;
		}

		if (bytesReceived == SOCKET_ERROR)
		{
			int error = WSAGetLastError();
			return EResult::NotYetImplemented;
		}
		senderEndpoint = IpEndpoint((sockaddr*)&senderAddr);

		return EResult::Success;
	}


	EResult UdpSocket::setSocketOption(ESocketOption socketOption, BOOL value)
	{
		return EResult::Success;
	}

}
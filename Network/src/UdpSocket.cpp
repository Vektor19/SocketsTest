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

	EResult UdpSocket::setSocketOption(ESocketOption socketOption, BOOL value)
	{
		return EResult::Success;
	}

}
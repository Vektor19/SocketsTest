#include "Socket.h"
#include <assert.h>
namespace networking {

	Socket::Socket(EIpVersion ipVersion, SocketHandle socketHandle)
		: m_ipVersion(ipVersion)
		, m_socketHandle(socketHandle)
	{
		assert(m_ipVersion == EIpVersion::IPv4 || m_ipVersion == EIpVersion::IPv6);
	}

	EResult Socket::close()
	{
		if (m_socketHandle == INVALID_SOCKET)
		{
			return EResult::NotYetImplemented;
		}
		int result = closesocket(m_socketHandle);
		if (result != 0)
		{
			int error = WSAGetLastError();
			return EResult::NotYetImplemented;
		}
		m_socketHandle = INVALID_SOCKET;
		return EResult::Success;
	}

	EResult Socket::bind(IpEndpoint endPoint)
	{
		if (m_ipVersion == EIpVersion::IPv4)
		{
			sockaddr_in addr = endPoint.getSockaddrIPv4();
			int result = ::bind(m_socketHandle, (sockaddr*)(&addr), sizeof(sockaddr_in));
			if (result != 0)
			{
				int error = WSAGetLastError();
				return EResult::NotYetImplemented;
			}
		}
		else
		{
			sockaddr_in6 addr = endPoint.getSockaddrIPv6();
			int result = ::bind(m_socketHandle, (sockaddr*)(&addr), sizeof(sockaddr_in6));
			if (result != 0)
			{
				int error = WSAGetLastError();
				return EResult::NotYetImplemented;
			}
		}
		
		return EResult::Success;
	}

	EIpVersion networking::Socket::getIpVersion()
	{
		return m_ipVersion;
	}

	SocketHandle Socket::getSocketHandle()
	{
		return m_socketHandle;
	}

}
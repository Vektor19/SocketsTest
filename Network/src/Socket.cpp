#include "Socket.h"
#include <assert.h>
namespace networking {

	Socket::Socket(EIpVersion ipVersion, SocketHandle socketHandle)
		: m_ipVersion(ipVersion)
		, m_socketHandle(socketHandle)
	{
		assert(m_ipVersion == EIpVersion::IPv4);
	}

	EResult Socket::create()
	{
		assert(m_ipVersion == EIpVersion::IPv4);
		if (m_socketHandle != INVALID_SOCKET)
		{
			return EResult::NotYetImplemented;
		}
		m_socketHandle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (m_socketHandle==INVALID_SOCKET)
		{
			int error = WSAGetLastError();
			return EResult::NotYetImplemented;
		}
		if (setSocketOption(TCP_NoDelay, TRUE) != EResult::Success)
		{
			return EResult::NotYetImplemented;
		}
		return EResult::Success;
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

	EIpVersion networking::Socket::getIpVersion()
	{
		return m_ipVersion;
	}

	SocketHandle Socket::getSocketHandle()
	{
		return m_socketHandle;
	}

	EResult Socket::setSocketOption(ESocketOption socketOption, BOOL value)
	{
		int result = 0;
		switch (socketOption)
		{
		case ESocketOption::TCP_NoDelay:
			result = setsockopt(m_socketHandle, IPPROTO_TCP, TCP_NODELAY, (const char*)&value, sizeof(value));
			break;
		default:
			return EResult::NotYetImplemented;
		}
		if (result != 0)
		{
			int error = WSAGetLastError();
			return EResult::NotYetImplemented;
		}
		return EResult::Success;
	}

}
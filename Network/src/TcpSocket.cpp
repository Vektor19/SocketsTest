#include "TcpSocket.h"
#include <assert.h>
#include <iostream>
namespace networking {

	TcpSocket::TcpSocket(EIpVersion ipVersion, SocketHandle socketHandle)
		: Socket(ipVersion, socketHandle)
	{
	}

	EResult TcpSocket::create()
	{
		assert(m_ipVersion == EIpVersion::IPv4 || m_ipVersion == EIpVersion::IPv6);
		if (m_socketHandle != INVALID_SOCKET)
		{
			return EResult::NotYetImplemented;
		}
		m_socketHandle = socket(m_ipVersion == EIpVersion::IPv4 ? AF_INET : AF_INET6, SOCK_STREAM, IPPROTO_TCP);
		if (m_socketHandle == INVALID_SOCKET)
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

	EResult TcpSocket::listen(IpEndpoint endpoint, int backlog)
	{
		if (m_ipVersion==EIpVersion::IPv6)
		{
			if (setSocketOption(ESocketOption::Ipv6_Only, FALSE) != EResult::Success)
			{
				return EResult::NotYetImplemented;
			}
		}

		if (this->bind(endpoint) != EResult::Success)
		{
			return EResult::NotYetImplemented;
		}
		int result = ::listen(m_socketHandle, backlog);
		if (result != 0)
		{
			int error = WSAGetLastError();
			return EResult::NotYetImplemented;
		}
		return EResult::Success;
	}

	EResult TcpSocket::accept(Socket& outSocket, IpEndpoint& outEndpoint)
	{
		if (m_ipVersion==IPv4)
		{
			sockaddr_in addr = {};
			int len = sizeof(sockaddr_in);
			SocketHandle acceptedSock = ::accept(m_socketHandle, (sockaddr*)(&addr), &len);
			if (acceptedSock == INVALID_SOCKET)
			{
				int error = WSAGetLastError();
				return EResult::NotYetImplemented;
			}
			outEndpoint = IpEndpoint((sockaddr*)&addr);
			outSocket = TcpSocket(IPv4, acceptedSock);
		}
		else
		{
			sockaddr_in6 addr = {};
			int len = sizeof(sockaddr_in6);
			SocketHandle acceptedSock = ::accept(m_socketHandle, (sockaddr*)(&addr), &len);
			if (acceptedSock == INVALID_SOCKET)
			{
				int error = WSAGetLastError();
				return EResult::NotYetImplemented;
			}
			outEndpoint = IpEndpoint((sockaddr*)&addr);
			outSocket = TcpSocket(IPv6, acceptedSock);
		}
		return EResult::Success;
	}

	EResult TcpSocket::connect(IpEndpoint endpoint)
	{
		int result = 0;
		if (m_ipVersion==IPv4)
		{
			sockaddr_in addr = endpoint.getSockaddrIPv4();
			result = ::connect(m_socketHandle, (sockaddr*)(&addr), sizeof(sockaddr_in));
		}
		else
		{
			sockaddr_in6 addr = endpoint.getSockaddrIPv6();
			result = ::connect(m_socketHandle, (sockaddr*)(&addr), sizeof(sockaddr_in6));
		}
		if (result != 0)
		{
			int error = WSAGetLastError();
			return EResult::NotYetImplemented;
		}
		return EResult::Success;
	}

	EResult TcpSocket::send(const void* data, int numberOfBytes, int& bytesSent)
	{
		bytesSent = ::send(m_socketHandle, (const char*)data, numberOfBytes, NULL);
		if (bytesSent == SOCKET_ERROR)
		{
			int error = WSAGetLastError();
			return EResult::NotYetImplemented;
		}
		return EResult::Success;
	}

	EResult TcpSocket::sendAll(const void* data, int numberOfBytes)
	{
		int totalBytesSent = 0;
		while (totalBytesSent < numberOfBytes)
		{
			int bytesRemaining = numberOfBytes - totalBytesSent;
			int bytesSent = 0;
			char* bufferOffset = (char*)data + totalBytesSent;
			EResult result = send(bufferOffset, bytesRemaining, bytesSent);
			if (result != EResult::Success)
			{
				return EResult::NotYetImplemented;
			}
			totalBytesSent += bytesSent;
		}
		return EResult::Success;
	}

	EResult TcpSocket::recv(void* destination, int numberOfBytes, int& bytesRecieved)
	{
		bytesRecieved = ::recv(m_socketHandle, (char*)destination, numberOfBytes, NULL);
		if (bytesRecieved == 0)
		{
			return EResult::NotYetImplemented;
		}
		if (bytesRecieved == SOCKET_ERROR)
		{
			int error = WSAGetLastError();
			return EResult::NotYetImplemented;
		}
		return EResult::Success;
	}

	EResult TcpSocket::recvAll(const void* destination, int numberOfBytes)
	{
		int totalBytesReceived = 0;
		while (totalBytesReceived < numberOfBytes)
		{
			int bytesRemaining = numberOfBytes - totalBytesReceived;
			int bytesReceived = 0;
			char* bufferOffset = (char*)destination + totalBytesReceived;
			EResult result = recv(bufferOffset, bytesRemaining, bytesReceived);
			if (result != EResult::Success)
			{
				return EResult::NotYetImplemented;
			}
			totalBytesReceived += bytesReceived;
		}
		return EResult::Success;
	}

	EResult TcpSocket::shutdown(EShutdownType eShutdownType)
	{
		int result = ::shutdown(m_socketHandle, (int)eShutdownType);
		if (result != 0)
		{
			int error = WSAGetLastError();
			return EResult::NotYetImplemented;
		}
		return EResult::Success;
	}

	EResult TcpSocket::setSocketOption(ESocketOption socketOption, BOOL value)
	{
		int result = 0;
		switch (socketOption)
		{
		case ESocketOption::TCP_NoDelay:
			result = setsockopt(m_socketHandle, IPPROTO_TCP, TCP_NODELAY, (const char*)&value, sizeof(value));
			break;
		case ESocketOption::Ipv6_Only:
			result = setsockopt(m_socketHandle, IPPROTO_IPV6, IPV6_V6ONLY, (const char*)&value, sizeof(value));
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
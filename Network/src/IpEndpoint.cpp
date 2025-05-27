#include <assert.h>
#include "IpEndpoint.h"
#include <iostream>
namespace networking {
	IpEndpoint::IpEndpoint(const char* ip, uint16_t port)
	{
		m_port = port;

		//ipv4
		in_addr addr;
		int result = inet_pton(AF_INET, ip, &addr);
		if (result == 1)
		{
			if (addr.S_un.S_addr != INADDR_NONE)
			{
				m_hostname = ip;
				m_ipStr = ip;
				m_ipVersion = EIpVersion::IPv4;
				m_ipBytes.resize(sizeof(ULONG));
				memcpy(&m_ipBytes[0], &addr.S_un.S_addr, sizeof(ULONG));
				return;
			}
		}

		addrinfo hints = {};
		hints.ai_family = AF_INET;
		addrinfo* hostInfo = nullptr;
		result = getaddrinfo(ip, NULL, &hints, &hostInfo);
		if (result == 0)
		{
			sockaddr_in* host_addr = reinterpret_cast<sockaddr_in*>(hostInfo->ai_addr);
			m_hostname = ip;
			m_ipStr.resize(16);
			inet_ntop(AF_INET, &host_addr->sin_addr.S_un.S_addr, &m_ipStr[0], 16);
			m_ipBytes.resize(sizeof(ULONG));
			memcpy(&m_ipBytes[0], &host_addr->sin_addr.S_un.S_addr, sizeof(ULONG));
			m_ipVersion = EIpVersion::IPv4;
			freeaddrinfo(hostInfo);
			return;
		}

		//ipv6
		in6_addr addr6;
		result = inet_pton(AF_INET6, ip, &addr6);
		if (result == 1)
		{
			m_hostname = ip;
			m_ipStr = ip;
			m_ipVersion = EIpVersion::IPv6;
			m_ipBytes.resize(16);
			memcpy(&m_ipBytes[0], &addr6.u, 16);
			return;
		}

		addrinfo hints6 = {};
		hints.ai_family = AF_INET6;
		addrinfo* hostInfo6 = nullptr;
		result = getaddrinfo(ip, NULL, &hints6, &hostInfo6);
		if (result == 0)
		{
			sockaddr_in6* host_addr = reinterpret_cast<sockaddr_in6*>(hostInfo6->ai_addr);
			m_hostname = ip;
			m_ipStr.resize(46);
			inet_ntop(AF_INET6, &host_addr->sin6_addr, &m_ipStr[0], 46);
			m_ipBytes.resize(16);
			memcpy(&m_ipBytes[0], &host_addr->sin6_addr, 16);
			m_ipVersion = EIpVersion::IPv6;
			freeaddrinfo(hostInfo6);
		}
	}

	IpEndpoint::IpEndpoint(sockaddr* addr)
	{
		assert(addr->sa_family == AF_INET || addr->sa_family == AF_INET6);

		if (addr->sa_family == AF_INET)
		{
			sockaddr_in* addrv4 = reinterpret_cast<sockaddr_in*>(addr);
			m_ipVersion = IPv4;
			m_port = ntohs(addrv4->sin_port);
			m_ipBytes.resize(sizeof(ULONG));
			memcpy(&m_ipBytes[0], &addrv4->sin_addr, sizeof(ULONG));
			m_ipStr.resize(16);
			m_ipStr = inet_ntop(AF_INET, &addrv4->sin_addr, &m_ipStr[0], 16);
			m_hostname = m_ipStr;
		}
		else
		{
			sockaddr_in6* addrv6 = reinterpret_cast<sockaddr_in6*>(addr);
			m_ipVersion = IPv6;
			m_port = ntohs(addrv6->sin6_port);
			m_ipBytes.resize(16);
			memcpy(&m_ipBytes[0], &addrv6->sin6_addr, 16);
			m_ipStr.resize(46);
			m_ipStr = inet_ntop(AF_INET6, &addrv6->sin6_addr, &m_ipStr[0], 46);
			m_hostname = m_ipStr;
		}
	}

	EIpVersion IpEndpoint::getIpVersion()
	{
		return m_ipVersion;
	}

	std::string networking::IpEndpoint::getHostname()
	{
		return m_hostname;
	}

	std::string IpEndpoint::getIpStr()
	{
		return m_ipStr;
	}

	std::vector<uint8_t> IpEndpoint::getIpBytes()
	{
		return m_ipBytes;
	}

	uint16_t IpEndpoint::getPort()
	{
		return m_port;
	}

	sockaddr_in IpEndpoint::getSockaddrIPv4()
	{
		assert(m_ipVersion = EIpVersion::IPv4);
		sockaddr_in addr = {};
		memcpy(&addr.sin_addr, &m_ipBytes[0], sizeof(ULONG));
		addr.sin_port = htons(m_port);
		addr.sin_family = AF_INET;
		return addr;
	}

	sockaddr_in6 IpEndpoint::getSockaddrIPv6()
	{
		assert(m_ipVersion = EIpVersion::IPv6);
		sockaddr_in6 addr6 = {};
		memcpy(&addr6.sin6_addr, &m_ipBytes[0], 16);
		addr6.sin6_port = htons(m_port);
		addr6.sin6_family = AF_INET6;
		return addr6;
	}

	void IpEndpoint::print()
	{
		switch (m_ipVersion)
		{
		case EIpVersion::IPv4:
			std::cout << "IP Version: IPv4" << std::endl;
			break;
		case EIpVersion::IPv6:
			std::cout << "IP Version: IPv6" << std::endl;
			break;
		default:
			std::cout << "IP Version: Unknown" << std::endl;
			break;
		}
		std::cout << "Hostname: " << m_hostname << std::endl;
		std::cout << "Ip: " << m_ipStr << std::endl;
		std::cout << "Port: " << m_port << std::endl;
		std::cout << "Ip Bytes: " << std::endl;
		for (auto& digit : m_ipBytes)
		{
			std::cout << (int)digit << std::endl;
		}
	}

}
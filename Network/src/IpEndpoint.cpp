#include <assert.h>
#include "IpEndpoint.h"
#include <iostream>
namespace networking {
	IpEndpoint::IpEndpoint(const char* ip, uint16_t port)
	{
		m_port = port;
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

	void IpEndpoint::print()
	{
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
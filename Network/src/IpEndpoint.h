#pragma once
#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include "SocketHandle.h"
#include "EIpVersion.h"
#include "EResult.h"
#include <stdint.h>
#include <string>
#include <vector>
namespace networking {
	class IpEndpoint
	{
	public:
		IpEndpoint(const char * ip, uint16_t port);

		EIpVersion getIpVersion();
		std::string getHostname();
		std::string getIpStr();
		std::vector<uint8_t> getIpBytes();
		uint16_t getPort();
	private:
		EIpVersion m_ipVersion = EIpVersion::Unknown;
		std::string m_hostname;
		std::string m_ipStr;
		std::vector<uint8_t> m_ipBytes;
		uint16_t m_port;
	};

}
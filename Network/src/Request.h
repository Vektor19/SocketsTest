#pragma once
#include "ParseResult.h"
#include <string>
#include <map>

namespace networking
{
	class Request
	{
	public:
		ParseResult parseFromString(std::string requestStr);
	private:
		std::string m_method;
		std::string m_uriPath;
		std::string m_httpVersion;
		std::map<std::string, std::string> m_headers;
		std::string m_body;
	};

}
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
		const std::string& getMethod() const { return m_method; }
		const std::string& getUriPath() const { return m_uriPath; }
		const std::string& getHttpVersion() const { return m_httpVersion; }
		const std::map<std::string, std::string>& getHeaders() const { return m_headers; }
	private:
		std::string m_method;
		std::string m_uriPath;
		std::string m_httpVersion;
		std::map<std::string, std::string> m_headers;
		std::string m_body;
	};

}
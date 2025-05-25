#pragma once
#include <string>
#include "ResponseStatus.h"
#include <map>
namespace networking {
	class Response
	{
	public:
		Response();
		std::string toString();
		void setStatus(ResponseStatus status);
		void addHeader(const std::string& key, const std::string& value);
		void setBody(std::string& body);
	private:
		ResponseStatus m_status;
		std::string m_httpVersion;
		std::map<std::string, std::string> m_headers;
		std::string m_body;
	};
}
#include "Request.h"
#include <iostream>
#include "StringUtils.h"

namespace networking
{
	ParseResult Request::parseFromString(std::string requestStr)
	{
		auto dividerIndex = requestStr.find("\r\n\r\n");
		if (dividerIndex == std::string::npos) return ParseResult::BadRequest;

		std::string headingContent = requestStr.substr(0, dividerIndex);
		std::cout << "REQUEST HEAD: \n" << headingContent;

		m_body = requestStr.substr(dividerIndex + 4);
		std::cout << "BODY: \n" << m_body;
		auto methodEndIndex = headingContent.find(' ');
		m_method = headingContent.substr(0, methodEndIndex);
		
		if (m_method != "GET" && m_method != "POST") return ParseResult::BadMethod;
		auto uriStartIndex = methodEndIndex + 1;
		auto uriEndIndex = headingContent.find(' ', uriStartIndex);
		m_uriPath = headingContent.substr(uriStartIndex, uriEndIndex - uriStartIndex);
		
		auto httpVersionStartIndex = uriEndIndex + 1;
		auto httpVersionEndIndex = headingContent.find('\r', httpVersionStartIndex);
		m_httpVersion = headingContent.substr(httpVersionStartIndex, httpVersionEndIndex - httpVersionStartIndex);

		std::string headersString = headingContent.substr(headingContent.find('\n') + 1);

		std::vector<std::string> headers = StringUtils::split(headersString, "\r\n");
		for (std::string& headerRow: headers)
		{
			auto keyEndIndex = headerRow.find(' ');
			m_headers.emplace(std::move(headerRow.substr(0, keyEndIndex-1)), std::move(headerRow.substr(keyEndIndex + 1)));
		}

		return ParseResult::OK;
	}
}
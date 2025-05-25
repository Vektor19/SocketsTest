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
		auto afterMethodIndex = headingContent.find(' ');
		m_method = headingContent.substr(0, afterMethodIndex);
		
		if (m_method != "GET" && m_method != "POST") return ParseResult::BadMethod;
		
		std::string headersString = headingContent.substr(afterMethodIndex + 1);


	}
}
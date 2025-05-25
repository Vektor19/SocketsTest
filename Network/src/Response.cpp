#include "Response.h"
namespace networking
{
	Response::Response()
	{
		m_httpVersion = "HTTP/1.1";
		m_status = ResponseStatus::OK;
		m_body = "";
	}
	std::string Response::toString()
	{
		std::string responseString = m_httpVersion +" ";
		std::string status;
		switch (m_status)
		{
		case ResponseStatus::OK:
			status = ResponseStatus::OK + " OK";
			break;
		case ResponseStatus::NotFound:
			status = ResponseStatus::NotFound + " Not Found";
			break;
		case ResponseStatus::MethodNotAllowed:
			status = ResponseStatus::MethodNotAllowed + " Method Not Allowed";
			break;
		case ResponseStatus::BadRequest:
			status = ResponseStatus::BadRequest + " Bad Request";
			break;
		case ResponseStatus::Created:
			status = ResponseStatus::Created + " Created";
			break;
		default:
			status = ResponseStatus::BadRequest + " Bad Request";
			break;
		}
		responseString.append("\r\n");
		for (auto& headerPair : m_headers)
		{
			responseString.append(headerPair.first + ": " + headerPair.second + "\r\n");
		}
		responseString.append("\r\n");
		responseString.append(m_body);
		return responseString;
	}
	void Response::setStatus(ResponseStatus status)
	{
		this->m_status = status;
	}
	void Response::addHeader(const std::string& key, const std::string& value)
	{
		m_headers.emplace(key, value);
	}
	void Response::setBody(std::string& body)
	{
		this->m_body = body;
	}
}
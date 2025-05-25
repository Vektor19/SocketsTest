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
		case OK:
			status = OK + " OK";
			break;
		case NotFound:
			status = NotFound + " Not Found";
			break;
		case MethodNotAllowed:
			status = MethodNotAllowed + " Method Not Allowed";
			break;
		case BadRequest:
			status = BadRequest + " Bad Request";
			break;
		case Created:
			status = Created + " Created";
			break;
		default:
			status = BadRequest + " Bad Request";
			break;
		}
		responseString.append("\r\n");
		for (auto& headerPair : m_headers)
		{
			responseString.append(headerPair.first + ": " + headerPair.second + "\r\n");
		}
		responseString.append("\r\n");
		responseString.append(m_body);
	}
	void Response::setStatus(ResponseStatus status)
	{
		this->m_status = status;
	}
	void Response::addHeader(std::string& key, std::string& value)
	{
		m_headers.emplace(key, value);
	}
	void Response::setBody(std::string& body)
	{
		this->m_body = body;
	}
}
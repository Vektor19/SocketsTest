#include "StringUtils.h"
#include <sstream>

std::vector<std::string> StringUtils::split(const std::string& str, char splitChar)
{
	std::vector<std::string> output;

	std::stringstream ss(str);
	std::string line;
	while (std::getline(ss, line, splitChar))
	{
		output.push_back(line);
	}
	return output;
}

std::vector<std::string> StringUtils::split(const std::string& str, const std::string& delimiter)
{
	std::vector<std::string> output;
	size_t prev = 0, pos = 0;
	while ((pos = str.find(delimiter, prev)) != std::string::npos)
	{
		output.push_back(str.substr(prev, pos - prev));
		prev = pos + delimiter.length();
	}
	output.push_back(str.substr(prev));
	return output;
}

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

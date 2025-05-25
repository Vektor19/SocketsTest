#pragma once
#include <vector>
#include <string>
class StringUtils
{
public:
	static std::vector<std::string> split(const std::string& str, char splitChar);
	static std::vector<std::string> split(const std::string& str, const std::string& delimiter);
};
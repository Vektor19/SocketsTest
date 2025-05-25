#include <fstream>
#include <iostream>
#include <sstream>
#include "IOUtils.h"

std::string IOUtils::getFileString(const std::string& absoluteFilePath)
{
	std::ifstream f;
	f.open(absoluteFilePath, std::ios::in | std::ios::binary);
	if (!f.is_open())
	{
		std::cerr << "Failed to open file: " << absoluteFilePath << std::endl;
		return std::string();
	}
	std::stringstream buffer;
	buffer << f.rdbuf();
	return buffer.str();
}

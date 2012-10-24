#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <fstream>
#include <algorithm>

#include "config.hpp"
#include "log.h"

namespace San2 { namespace Utils { namespace Config {

void ConfigFile::removeComment(std::string &line) const
{
	if (line.find(';') != line.npos) line.erase(line.find(';'));
}

bool ConfigFile::onlyWhitespace(const std::string &line) const
{
	return (line.find_first_not_of(' ') == line.npos);
}

void ConfigFile::extractKey(std::string &key, size_t const &sepPos, const std::string &line) const
{
	key = line.substr(0, sepPos);
	if (key.find('\t') != line.npos || key.find(' ') != line.npos) key.erase(key.find_first_of("\t "));
}

void ConfigFile::extractValue(std::string &value, size_t const &sepPos, const std::string &line) const
{
	value = line.substr(sepPos + 1);
	value.erase(0, value.find_first_not_of("\t "));
	value.erase(value.find_last_not_of("\t ") + 1);
}

void ConfigFile::extractContents(std::string line)
{
	line.erase(0, line.find_first_not_of("\t "));
	size_t separatorPosition = line.find('=');

	std::string key, value;
	extractKey(key, separatorPosition, line);
	extractValue(value, separatorPosition, line);

	std::transform(key.begin(), key.end(), key.begin(), ::tolower);

	// last value wins! :)
	if (keyExists(key)) contents[key] = value; // update value
	else contents.insert(std::pair<std::string, std::string>(key, value)); // new key
}

bool ConfigFile::validLine(const std::string &line) const
{
	std::string temp = line;
	temp.erase(0, temp.find_first_not_of("\t "));
	if (temp[0] == '=')	return false;
	for (size_t i = temp.find('=') + 1; i < temp.length(); i++) if (temp[i] != ' ')	return true;
	return false;
}

bool ConfigFile::parseLine(const std::string &line, size_t const lineNo)
{
	if (line.find('=') == line.npos) return false;
	if (!validLine(line))return false;

	extractContents(line);
	return true;
}

// return value: true = SUCCESS
bool ConfigFile::load(const std::string &filename, bool strictParsing)
{
	fName = filename;
	std::ifstream file;
	file.open(fName.c_str());
	if (!file.good()) return false;

	std::string line;
	size_t lineNo = 0;
	
	while (std::getline(file, line)) // foreach line
	{
		lineNo++;
		std::string temp = line;
		if (temp.empty()) continue;
		removeComment(temp);
		if (onlyWhitespace(temp)) continue;
		bool ret = parseLine(temp, lineNo);
		if ((ret == false && strictParsing == true)) return false;
	}

	file.close();
	return true;
}

bool ConfigFile::keyExists(std::string key) const
{
	std::transform(key.begin(), key.end(), key.begin(), ::tolower);
	return contents.find(key) != contents.end();
}

std::string ConfigFile::getValue(std::string key)
{
	std::transform(key.begin(), key.end(), key.begin(), ::tolower);
	if (!keyExists(key)) return std::string("");
	return contents.find(key)->second;
}

}}} // ns

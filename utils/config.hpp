#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <fstream>

namespace San2 
{ 
	namespace Utils 
	{ 
		namespace Config 
		{
			class ConfigFile
			{
			public:
				bool keyExists(std::string key) const;
				bool load(const std::string &filename, bool strictParsing = true);	
				std::string getValue(std::string key);

			private:
				std::map<std::string, std::string> contents;
				std::string fName;

				void removeComment(std::string &line) const;
				bool onlyWhitespace(const std::string &line) const;
				bool validLine(const std::string &line) const;
				void extractKey(std::string &key, size_t const &sepPos, const std::string &line) const;
				void extractValue(std::string &value, size_t const &sepPos, const std::string &line) const;
				void extractContents(std::string line);
				bool parseLine(const std::string &line, size_t const lineNo);
			};
		}
	}
}


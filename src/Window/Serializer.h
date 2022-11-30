#pragma once
#include <regex>
#include <string>
#include <vector>

class Serializer {
public:
	Serializer();
	virtual ~Serializer();

	virtual bool find(std::vector<std::string>& files);
	virtual bool read(const std::string& filename) = 0;
	virtual bool write(const std::string& filename) = 0;
	bool remove(const std::string& filename);

	virtual const char* getFileExtension() = 0;
protected:
	bool find_(std::vector<std::string>& files, const std::regex& pattern);
	bool read_(const char* filename, void* data, std::streamsize size, unsigned int amount);
	bool write_(const char* filename, void* data, std::streamsize size, unsigned int amount);

	const std::string& cConfigRootDir = "Saves/";
};

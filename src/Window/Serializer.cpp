#include "Serializer.h"

#include <filesystem>
#include <fstream>
#include <iostream>

Serializer::Serializer() {
}

Serializer::~Serializer() {
}

bool Serializer::find(std::vector<std::string>& files) {
	const std::regex cFileRegex(std::string(cConfigRootDir).append(R"(([a-zA-Z0-9_-]+)\.)").append(getFileExtension()));
	return find_(files, cFileRegex);
}

bool Serializer::remove(const std::string& filename) {
	return std::filesystem::remove(cConfigRootDir + filename + "." + getFileExtension());
}

bool Serializer::find_(std::vector<std::string>& files, const std::regex& pattern) {
	std::filesystem::create_directory(cConfigRootDir);
	for (const auto& entry : std::filesystem::directory_iterator(cConfigRootDir)) {
		std::string path((const char*)entry.path().u8string().c_str());
		std::smatch matches;
		if (std::regex_search(path, matches, pattern)) {
			std::string file = matches[1];
			files.emplace_back(file);
		}
	}
	return true;
}

bool Serializer::read_(const char* filename, void* data, std::streamsize size, unsigned int amount) {
	std::ifstream stream(cConfigRootDir + filename + "." + getFileExtension(), std::ios::out | std::ios::binary);
	if (!stream)
		return false;

	stream.read((char*)data, size * amount);

	stream.close();
	if (!stream.good())
		return false;
	return true;
}

bool Serializer::write_(const char* filename, void* data, std::streamsize size, unsigned int amount) {
	std::ofstream stream(cConfigRootDir + filename + "." + getFileExtension(), std::ios::out | std::ios::binary);
	if (!stream)
		return false;

	stream.write((char*)data, size * amount);

	stream.close();
	if (!stream.good())
		return false;
	return true;
}

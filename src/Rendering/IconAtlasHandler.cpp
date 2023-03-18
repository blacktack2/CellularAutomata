#include "IconAtlasHandler.h"

#include <fstream>
#include <sstream>

std::map<std::string, IconAtlasHandler::AtlasMap> IconAtlasHandler::sAtlasMaps{};

const std::string IconAtlasHandler::cFilePrefix = "IconAtlas_";
const std::string IconAtlasHandler::cAtlasDir   = "Resources/AtlasMaps/";
const std::string IconAtlasHandler::cAtlasExt   = ".map";

IconAtlasHandler::AtlasMap* IconAtlasHandler::loadAtlasMap(std::string name) {
	ImageLoader::Image* image = ImageLoader::getImage(cFilePrefix + name);
	if (image == nullptr)
		return nullptr;
	AtlasMap& map = sAtlasMaps.emplace(name, AtlasMap{*image}).first->second;
	float invWidth = 1.0f / image->width;
	float invHeight = 1.0f / image->height;

	std::ifstream stream(cAtlasDir + cFilePrefix + name + cAtlasExt);

	std::string line;
	while (std::getline(stream, line)) {
		std::istringstream sstream(line);
		std::string token;
		std::getline(sstream, token, ',');
		Icon& icon = map.icons.emplace(token, Icon()).first->second;

		std::getline(sstream, token, ',');
		float u1 = std::stoi(token) * invWidth;
		std::getline(sstream, token, ',');
		float v1 = std::stoi(token) * invHeight;
		std::getline(sstream, token, ',');
		float u2 = u1 + std::stoi(token) * invWidth;
		std::getline(sstream, token, ',');
		float v2 = v1 + std::stoi(token) * invHeight;

		icon.uv1 = ImVec2(u1, v1);
		icon.uv2 = ImVec2(u2, v2);
	}

	stream.close();

	return &map;
}

IconAtlasHandler::AtlasMap* IconAtlasHandler::getAtlasMap(std::string name) {
	auto map = sAtlasMaps.find(name);
	return map == sAtlasMaps.end() ? nullptr : &map->second;
}

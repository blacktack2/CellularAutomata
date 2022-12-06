#pragma once
#include "ImageLoader.h"

#include <imgui/imgui.h>

#include <map>
#include <string>

class IconAtlasHandler {
public:
	struct Icon {
		ImVec2 uv1;
		ImVec2 uv2;
	};
	struct AtlasMap {
		ImageLoader::Image& atlas;
		std::map<std::string, Icon> icons;
	};

	static AtlasMap* loadAtlasMap(std::string name);
	static AtlasMap* getAtlasMap(std::string name);
private:
	static std::map<std::string, AtlasMap> sAtlasMaps;

	static const std::string cFilePrefix;
	static const std::string cAtlasDir;
	static const std::string cAtlasExt;
};
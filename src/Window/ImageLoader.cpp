#include "ImageLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

std::map<std::string, ImageLoader::Image> ImageLoader::sImages{};

const std::string ImageLoader::cImageDir = "Resources/Images/";
const std::string ImageLoader::cImageExt = ".png";

ImageLoader::Image* ImageLoader::getImage(std::string name) {
	auto atlasPair = sImages.find(name);
	return atlasPair == sImages.end() ? loadImage(name) : &atlasPair->second;
}

ImageLoader::Image* ImageLoader::loadImage(const std::string& name) {
	Image& out = sImages.emplace(name, Image(0, 0, 0)).first->second;
	stbi_uc* imageData = stbi_load((cImageDir + name + cImageExt).c_str(), &out.width, &out.height, nullptr, 4);
	if (imageData == nullptr)
		return nullptr;
	glGenTextures(1, &out.id);
	glBindTexture(GL_TEXTURE_2D, out.id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, out.width, out.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
	stbi_image_free(imageData);

	glBindTexture(GL_TEXTURE_2D, 0);

	return &out;
}

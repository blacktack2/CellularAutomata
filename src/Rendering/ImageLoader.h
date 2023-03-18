#pragma once
#include <glad/glad.h>

#include <map>
#include <string>

class ImageLoader {
public:
	struct Image {
		int width;
		int height;
		GLuint id;
	};
	static Image* getImage(std::string name);
private:
	static Image* loadImage(const std::string& name);

	static std::map<std::string, Image> sImages;

	static const std::string cImageDir;
	static const std::string cImageExt;
};
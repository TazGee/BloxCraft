#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb_image.h>

class Texture {
public:
	int id;
	unsigned int texture;
	const char* name;

	Texture();
	Texture(const char* path, const char* name, bool defaultParams = true);

	void generate();
	void load(bool flip = true);

	void SetFilters(GLenum all);
	void SetFilters(GLenum mag, GLenum min);

	void SetWrap(GLenum all);
	void SetWrap(GLenum s, GLenum t);

	void BindTexture();

private:
	static int currentID;
	const char* path;

	int width, height, nChannels;
};

#endif
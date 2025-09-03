#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include <streambuf>
#include <string>
#include <iostream>

class Shader {
public:
	unsigned int id;
	Shader(const char* vertexShaderPath, const char* fragmentShaderPath);
	Shader() : id(0) {}
	void Activate();

	//Utility
	std::string LoadShaderSource(const char* filepath);
	GLuint CompileShader(const char* filepath, GLenum type);

	void SetMat4(const std::string& name, glm::mat4 val);
};

#endif

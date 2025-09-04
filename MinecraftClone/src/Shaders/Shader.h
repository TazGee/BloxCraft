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
	void setBool(const std::string& name, bool value);
	void setInt(const std::string& name, int value);
	void setFloat(const std::string& name, float value);
	void set3Float(const std::string& name, float v1, float v2, float v3);
	void set3Float(const std::string& name, glm::vec3 v);
	void set4Float(const std::string& name, float v1, float v2, float v3, float v4);
	//void set4Float(const std::string& name, aiColor4D color);
	void set4Float(const std::string& name, glm::vec4 v);
	void setMat3(const std::string& name, glm::mat3 val);
};

#endif

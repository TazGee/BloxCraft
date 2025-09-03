#include "Shader.h"

Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	int success;
	char infoLog[512];
	unsigned int vertexShader = CompileShader("MinecraftClone/assets/vertex_core.glsl", GL_VERTEX_SHADER);
	unsigned int fragmentShader = CompileShader("MinecraftClone/assets/fragment_core.glsl", GL_FRAGMENT_SHADER);

	id = glCreateProgram();
	glAttachShader(id, vertexShader);
	glAttachShader(id, fragmentShader);
	glLinkProgram(id);

	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		std::cerr << "Linking error! Info: " << infoLog << std::endl;
	}
	else
		std::cout << "Shaders are successfully attached and the program is successfully linked!" << std::endl;

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Activate()
{
	glUseProgram(id);
}

std::string Shader::LoadShaderSource(const char* filename)
{
	std::ifstream file;
	std::stringstream buf;

	std::string ret = "";

	file.open(filename);

	if (file.is_open())
	{
		buf << file.rdbuf();
		ret = buf.str();
	}
	else
	{
		std::cerr << "Could not load a file: " << filename << std::endl;
	}

	file.close();
	return ret;
}

GLuint Shader::CompileShader(const char* shaderPath, GLenum shtype)
{
	int success;
	char infoLog[512];
	std::cout << "Compiling shader at path: " << shaderPath << "...";
	GLuint shader;

	shader = glCreateShader(shtype);

	std::string shaderSrc = LoadShaderSource(shaderPath);
	const GLchar* shdr = shaderSrc.c_str();

	glShaderSource(shader, 1, &shdr, NULL);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cerr << "Shader compile error! Info: " << infoLog << std::endl;
	}
	else
	{
		std::cout << " DONE!" << std::endl;
	}
	return shader;
}

void Shader::SetMat4(const std::string& name, glm::mat4 val) {
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(val));
}
#include "Shader.h"

Shader::Shader() { id = 0; }

Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	generate(vertexShaderPath, fragmentShaderPath);
}

void Shader::generate(const char* vertexShaderPath, const char* fragmentShaderPath)
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

void Shader::setBool(const std::string& name, bool value) {
	glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) {
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) {
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::set3Float(const std::string& name, float v1, float v2, float v3) {
	glUniform3f(glGetUniformLocation(id, name.c_str()), v1, v2, v3);
}

void Shader::set3Float(const std::string& name, glm::vec3 v) {
	glUniform3f(glGetUniformLocation(id, name.c_str()), v.x, v.y, v.z);
}

void Shader::set4Float(const std::string& name, float v1, float v2, float v3, float v4) {
	glUniform4f(glGetUniformLocation(id, name.c_str()), v1, v2, v3, v4);
}

/*void Shader::set4Float(const std::string& name, aiColor4D color) {
	glUniform4f(glGetUniformLocation(id, name.c_str()), color.r, color.g, color.b, color.a);
}*/

void Shader::set4Float(const std::string& name, glm::vec4 v) {
	glUniform4f(glGetUniformLocation(id, name.c_str()), v.x, v.y, v.z, v.w);
}

void Shader::setMat3(const std::string& name, glm::mat3 val) {
	glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(val));
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
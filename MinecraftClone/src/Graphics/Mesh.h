#ifndef  MESH_H
#define MESH_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>

#include "Shader.h"
#include "Texture.h"

struct Vertex {
	glm::vec3 pos;
	glm::vec2 texCoord;

	static std::vector<Vertex> genList(float * vertices, int noVertices);
};
typedef struct Vertex Vertex;

class Mesh {
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	unsigned int VAO;

	Mesh();
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

	void Render(Shader shader);

	void CleanUp();

private:
	unsigned int VBO, EBO;

	void Setup();
};

#endif


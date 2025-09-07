#ifndef GRASS_HPP
#define GRASS_HPP

#include "MinecraftClone/src/Graphics/Model.h"

class Grass : public Model {
public:
	glm::vec3 pos;
	glm::vec3 size;

	Grass(glm::vec3 pos, glm::vec3 size) : pos(pos), size(size)
	{

	}

	void Initialize()
	{
		int noVertices = 36;

		//Vertices
		float vertices[] = {
			// === Front face (1,1) ===
			-0.5f, -0.5f,  0.5f,  0.25f, 0.666f,
			 0.5f, -0.5f,  0.5f,  0.50f, 0.666f,
			 0.5f,  0.5f,  0.5f,  0.50f, 0.3333f,
			 0.5f,  0.5f,  0.5f,  0.50f, 0.3333f,
			-0.5f,  0.5f,  0.5f,  0.25f, 0.3333f,
			-0.5f, -0.5f,  0.5f,  0.25f, 0.666f,

			// === Back face (3,1) ===
			 0.5f, -0.5f, -0.5f,  0.75f, 0.666f,
			-0.5f, -0.5f, -0.5f,  1.00f, 0.666f,
			-0.5f,  0.5f, -0.5f,  1.00f, 0.3333f,
			-0.5f,  0.5f, -0.5f,  1.00f, 0.3333f,
			 0.5f,  0.5f, -0.5f,  0.75f, 0.3333f,
			 0.5f, -0.5f, -0.5f,  0.75f, 0.666f,

			 // === Left face (0,1) ===
			 -0.5f, -0.5f, -0.5f,  0.00f, 0.666f,
			 -0.5f, -0.5f,  0.5f,  0.25f, 0.666f,
			 -0.5f,  0.5f,  0.5f,  0.25f, 0.3333f,
			 -0.5f,  0.5f,  0.5f,  0.25f, 0.3333f,
			 -0.5f,  0.5f, -0.5f,  0.00f, 0.3333f,
			 -0.5f, -0.5f, -0.5f,  0.00f, 0.666f,

			 // === Right face (2,1) ===
			  0.5f, -0.5f,  0.5f,  0.50f, 0.666f,
			  0.5f, -0.5f, -0.5f,  0.75f, 0.666f,
			  0.5f,  0.5f, -0.5f,  0.75f, 0.3333f,
			  0.5f,  0.5f, -0.5f,  0.75f, 0.3333f,
			  0.5f,  0.5f,  0.5f,  0.50f, 0.3333f,
			  0.5f, -0.5f,  0.5f,  0.50f, 0.666f,

			  // === Top face (1,0) ===
			  -0.5f,  0.5f, -0.5f,  0.25f, 0.3333f,
			   0.5f,  0.5f, -0.5f,  0.50f, 0.3333f,
			   0.5f,  0.5f,  0.5f,  0.50f, 0.0000f,
			   0.5f,  0.5f,  0.5f,  0.50f, 0.0000f,
			  -0.5f,  0.5f,  0.5f,  0.25f, 0.0000f,
			  -0.5f,  0.5f, -0.5f,  0.25f, 0.3333f,

			  // === Bottom face (1,2) ===
			  -0.5f, -0.5f,  0.5f,  0.25f, 1.0000f,
			   0.5f, -0.5f,  0.5f,  0.50f, 1.0000f,
			   0.5f, -0.5f, -0.5f,  0.50f, 0.666f,
			   0.5f, -0.5f, -0.5f,  0.50f, 0.666f,
			  -0.5f, -0.5f, -0.5f,  0.25f, 0.666f,
			  -0.5f, -0.5f,  0.5f,  0.25f, 1.0000f
		};



		std::vector<unsigned int> indices(noVertices);
		for (unsigned int i = 0; i < noVertices; i++)
		{
			indices[i] = i;
		}

		Texture tex0("MinecraftClone/assets/images/grass.jpg", "texture0");
		tex0.load(false);

		meshes.push_back(Mesh(Vertex::genList(vertices, noVertices), indices, { tex0 }));
	}

	void Render(Shader shader)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, pos);
		model = glm::scale(model, size);
		model = glm::rotate(model, 0.0f, glm::vec3(0.5f));
		shader.SetMat4("model", model);

		Model::Render(shader);
	}
};

#endif // !CUBE_HPP

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include <streambuf>
#include <string>
#include <vector>

#include "Shaders/Shader.h"
#include <stb_image.h>

#include "IO/Keyboard.h"
#include "IO/Mouse.h"

int width, height;
float x, y, z;

// Kad se prozor resize-uje
void framebuffer_size_callback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
	width = w;
	height = h;
}								// Sirina i visina renderer-a


// Klasa renderer
class Renderer {
public:
	bool gameRunning;							// Odredjuje stanje igre

private:
	GLFWwindow* window;							// Pokazivac na prozorcic igrice
	unsigned int VAO, VBO, EBO;					// OpenGL buffer-i
	std::vector<Shader> shaderCollection;		// Vektor koji sadrzi sve Shader-e koji se renderuju

	glm::mat4 trans;							// Matrica za transformaciju
	unsigned int texture1;						// Tekstura


	

public:
	Renderer(int w, int h) : gameRunning(true)
	{
		width = w;
		height = h;

		//Init GLFW
		std::cout << "Initializing GLFW...";
		glfwInit();
		std::cout << " DONE!" << std::endl;

		//Hitn-ovanje da je OpenGL V3.3
		HintGLFW();

		//MAC Kompatibilnost
#ifdef __APPLE__
		InitApple();
#endif

		//Init window
		InitializeGameWindow();

		//Podesava fokus na window
		glfwMakeContextCurrent(window);

		//Provera da li se GLAD ucitao
		CheckGlad();

		//Podesavanje viewport-a
		SetUpViewport();

		glEnable(GL_DEPTH_TEST);

		//Vertices
		float vertices[] = {
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		};

		//VAO, VBO
		VaoVboEboSetup();

		//Input callbacks
		glfwSetKeyCallback(window, Keyboard::keyCallback);
		glfwSetCursorPosCallback(window, Mouse::cursorPosCallback);
		glfwSetMouseButtonCallback(window, Mouse::mouseButtonCallback);
		glfwSetScrollCallback(window, Mouse::mouseWheelCallback);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

		//POS
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		//CLR
		//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		//glEnableVertexAttribArray(1);

		//TXTRS
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glGenTextures(1, &texture1);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		int he, wi, nChannels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load("MinecraftClone/assets/images/tx1.jpg", &wi, &he, &nChannels, 0);

		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wi, he, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cerr << "Failed to load texture!" << std::endl;
		}

		stbi_image_free(data);


		//Shader-i
		Shader sh("MinecraftClone/assets/vertex_core.glsl", "MinecraftClone/assets/fragment_core.glsl");
		AddShader(sh);

		//Transformacije
		x = 0.0f;
		y = 0.0f;
		z = 3.0f;
		
		sh.Activate();
		sh.setInt("texture1", 0);

		std::cout << "\n=-------------------------------=\nRenderer started successfully!\n=-------------------------------=" << std::endl;
	}

	void AddShader(Shader sh) // Dodavanje Shader-a u kolekciju
	{
		shaderCollection.push_back(sh);
	}
	void RemoveShader(Shader sh) // Brisanje Shader-a iz kolekcije
	{
		//shaderCollection.erase(std::remove(shaderCollection.begin(), shaderCollection.end(), sh),shaderCollection.end());
	}

	void RenderFrame() // Metoda koja sluzi za renderovanje frame-a
	{
		// Procesuiranje input-a
		processInput();

		if (glfwWindowShouldClose(window)) TerminateGame(); // Zatvaranje prozorcica ukoliko je to zatrazeno

		// Ciscenje buffer-a za boju
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Bind-ovanje teksture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		// Crtanje elemenata
		glBindVertexArray(VAO);

		// Transformacije
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);

		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(-55.0f), glm::vec3(0.5f));
		view = glm::translate(view, glm::vec3(-x, -y, -z));
		projection = glm::perspective(glm::radians(70.0f), (float)width / (float)height, 0.1f, 100.0f);

		// Rad nad Shader-ima
		for (int i = 0; i < shaderCollection.size(); i++)
		{
			shaderCollection[i].Activate();
			shaderCollection[i].SetMat4("transform", trans);
			shaderCollection[i].SetMat4("model", model);
			shaderCollection[i].SetMat4("view", view);
			shaderCollection[i].SetMat4("projection", projection);
		}

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Zamena buffer-a
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	void TerminateGame() // Metoda za gasenje igre
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		//glDeleteVertexArrays(1, &EBO);
		gameRunning = false;
		glfwTerminate();
	}


private:

	void InitApple()
	{
		std::cout << "Initializing MAC compatibility...";
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		std::cout << " DONE!" << std::endl;
	}

	void SetUpViewport()
	{
		std::cout << "Setting the viewport...";
		glViewport(0, 0, width, height);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		std::cout << " DONE!" << std::endl;
	}

	void CheckGlad()
	{
		std::cout << "Initializing GLAD...";
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cerr << "Failed to initialize GLAD!" << std::endl;
			TerminateGame();
		}
		std::cout << " DONE!" << std::endl;
	}

	void InitializeGameWindow()
	{
		std::cout << "Initializing window...";
		window = glfwCreateWindow(width, height, "Game", NULL, NULL);
		if (window == NULL)
		{
			std::cerr << "Failed to initialize GLFW window!" << std::endl;
			TerminateGame();
		}
		std::cout << " DONE!" << std::endl;
	}

	void HintGLFW()
	{
		std::cout << "Hinting GLFW...";
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		std::cout << " DONE!" << std::endl;
	}

	void VaoVboEboSetup()
	{
		std::cout << "Setting up VAO and VBO...";
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		//glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		std::cout << " DONE!" << std::endl;
	}

	void processInput()
	{
		if (Keyboard::key(GLFW_KEY_ESCAPE))
			gameRunning = false;

		if (Keyboard::key(GLFW_KEY_W))
			z -= 0.02f;
		if (Keyboard::key(GLFW_KEY_S))
			z += 0.02f;
		if (Keyboard::key(GLFW_KEY_A))
			x -= 0.02f;
		if (Keyboard::key(GLFW_KEY_D))
			x += 0.02f;
	}
};


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

// FRAME SIZE BUFFER --------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

class Renderer {

// GAME RUNNING BOOLEAN -----------------------
public:
	bool gameRunning;

// ATTRIBUTES ---------------------------------
private:
	int width, height;
	GLFWwindow* window;
	unsigned int VAO, VBO, EBO;
	//glm::mat4 trans;
	std::vector<Shader> shaderCollection;
	unsigned int texture1;
// CONSTRUCTOR --------------------------------
public:
	Renderer(int width, int height) : width(width), height(height), gameRunning(true)
	{
		//Init GLFW
		std::cout << "Initializing GLFW...";
		glfwInit();
		std::cout << " DONE!" << std::endl;

		//Hitning that OpenGL is V3.3
		HintGLFW();

		//MAC Compatibility
#ifdef __APPLE__
		InitApple();
#endif

		//Init window
		InitializeGameWindow();

		//Sets the focus on the window
		glfwMakeContextCurrent(window);

		//If GLAD doesn't load
		CheckGlad();

		//Viewport settings
		SetUpViewport();


		//Vertices
		float vertices[] = {
			-0.5f, -0.5f, 0.0f,		1.0f, 1.0f, 0.5f,		0.0f, 0.0f,
			-0.5f, 0.5f, 0.0f,		0.5f, 1.0f, 0.75f,		0.0f, 1.0f,
			0.5f, -0.5f, 0.0f,		0.6f, 1.0f, 0.2f,		1.0f, 0.0f,
			0.5f, 0.5f, 0.0f,		1.0f, 0.2f, 1.0f,		1.1f, 1.1f
		};
		unsigned int indicies[] = {
			0, 1, 2,
			3, 1, 2
		};

		//VAO, VBO
		VaoVboEboSetup();

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

		//POS
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		//CLR
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		//TXTRS
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glGenTextures(1, &texture1);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		int h, w, nChannels;
		unsigned char* data = stbi_load("MinecraftClone/assets/images/img.jpg", &w, &h, &nChannels, 0);

		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cerr << "Failed to load texture!" << std::endl;
		}

		stbi_image_free(data);


		//Shaders
		Shader sh("MinecraftClone/assets/vertex_core.glsl", "MinecraftClone/assets/fragment_core.glsl");
		AddShader(sh);
		//Transformation
		//trans = glm::mat4(1.0f);
		//trans = glm::rotate(trans, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		sh.Activate();
		sh.setInt("texture1", 0);
		//sh.SetMat4("transform", trans);

		std::cout << "\n=-------------------------------=\nRenderer started successfully!\n=-------------------------------=" << std::endl;
	}

// PUBLIC METHODES ------------------------
	void AddShader(Shader sh)
	{
		shaderCollection.push_back(sh);
	}
	void RemoveShader(Shader sh)
	{
		//shaderCollection.erase(std::remove(shaderCollection.begin(), shaderCollection.end(), sh),shaderCollection.end());
	}

	void RenderFrame()
	{
		processInput(); //Process the input

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		for (int i = 0; i < shaderCollection.size(); i++)
		{
			//trans = glm::rotate(trans, glm::radians((float)glfwGetTime() / 100.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			//shaderCollection[i].Activate();
			//shaderCollection[i].SetMat4("transform", trans);
		}

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	void TerminateGame()
	{
		gameRunning = false;
		glfwTerminate();
	}

// PRIVATE METHODES ------------------------
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
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //Calls when window is being resized
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
		//If GLFW windows doesn't init
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
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		std::cout << " DONE!" << std::endl;
	}

	void processInput()
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			gameRunning = false;
	}
};


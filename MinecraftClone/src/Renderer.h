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

#include "Graphics/Shader.h"
#include "Graphics/Texture.h"

#include "MinecraftClone/assets/Models/Cube.hpp"

#include "IO/Keyboard.h"
#include "IO/Mouse.h"

#include "Player/Camera.h"

int width, height;

// Kad se prozor resize-uje
void framebuffer_size_callback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
	width = w;
	height = h;
}

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Klasa renderer
class Renderer {
public:
	bool gameRunning;							// Odredjuje stanje igre

private:
	GLFWwindow* window;							// Pokazivac na prozorcic igrice
	unsigned int VAO, VBO, EBO;					// OpenGL buffer-i
	
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
		glfwSwapInterval(0);

		//Provera da li se GLAD ucitao
		CheckGlad();

		//Podesavanje viewport-a
		SetUpViewport();

		glEnable(GL_DEPTH_TEST);

		//Input callbacks i sakrivanje kursora
		glfwSetKeyCallback(window, Keyboard::keyCallback);
		glfwSetCursorPosCallback(window, Mouse::cursorPosCallback);
		glfwSetMouseButtonCallback(window, Mouse::mouseButtonCallback);
		glfwSetScrollCallback(window, Mouse::mouseWheelCallback);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		//Shader-i
		Shader sh("MinecraftClone/assets/vertex_core.glsl", "MinecraftClone/assets/fragment_core.glsl");

		Cube cube(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(1.0f));
		cube.Initialize();

		std::cout << "\n=-------------------------------=\nRenderer started successfully!\n=-------------------------------=" << std::endl;

		while(gameRunning) // Main while petlja koja renderuje frejmove
		{
			// Racunanje deltaTime
			float currentTime = (float)glfwGetTime();
			deltaTime = currentTime - lastFrame;
			lastFrame = currentTime;

			// Procesuiranje input-a
			processInput();

			if (glfwWindowShouldClose(window)) TerminateGame(); // Zatvaranje prozorcica ukoliko je to zatrazeno

			// Ciscenje buffer-a za boju
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Transformacije
			glm::mat4 view = glm::mat4(1.0f);
			glm::mat4 projection = glm::mat4(1.0f);

			//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(-55.0f), glm::vec3(0.5f));
			view = camera.getViewMatrix();
			projection = glm::perspective(glm::radians(camera.zoom), (float)width / (float)height, 0.1f, 100.0f);

			// Rad nad Shader-om
			sh.Activate();
			sh.SetMat4("view", view);
			sh.SetMat4("projection", projection);

			cube.Render(sh);

			// Zamena buffer-a
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	void TerminateGame() // Metoda za gasenje igre
	{
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
			camera.updateCameraPos(CameraDirection::FORWARD, deltaTime);
		if (Keyboard::key(GLFW_KEY_S))
			camera.updateCameraPos(CameraDirection::BACKWARD, deltaTime);
		if (Keyboard::key(GLFW_KEY_A))
			camera.updateCameraPos(CameraDirection::LEFT, deltaTime);
		if (Keyboard::key(GLFW_KEY_D))
			camera.updateCameraPos(CameraDirection::RIGHT, deltaTime);
		if (Keyboard::key(GLFW_KEY_SPACE))
			camera.updateCameraPos(CameraDirection::UP, deltaTime);
		if (Keyboard::key(GLFW_KEY_LEFT_SHIFT))
			camera.updateCameraPos(CameraDirection::DOWN, deltaTime);

		double dx = Mouse::getDX(), dy = Mouse::getDY();
		if (dx != 0 || dy != 0)
		{
			camera.updateCameraDirection(dx, dy);
		}

		double scrollDy = Mouse::getScrollDY();
		if (scrollDy != 0)
		{
			camera.updateCameraZoom(scrollDy);
		}
	}
};


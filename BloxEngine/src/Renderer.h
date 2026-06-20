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
#include "Graphics/Light.h"

#include "BloxEngine/assets/Models/Cube.hpp"
#include "BloxEngine/assets/Models/Grass.hpp"
#include "BloxEngine/assets/Models/Lamp.hpp"

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

Camera camera(glm::vec3(0.0f, 2.0f, 4.0f));
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Klasa renderer
class Renderer {
public:
	bool gameRunning;							// Odredjuje stanje igre

private:
	GLFWwindow* window;							// Pokazivac na prozorcic igrice
	
public:
	Renderer(int w, int h) : gameRunning(true)
	{
		width = w;
		height = h;

		//Init GLFW
		std::cout << "Initializing GLFW...";
		glfwInit();
		std::cout << " DONE!" << std::endl;

		//Hint-ovanje da je OpenGL V3.3
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
		Shader sh("BloxEngine/assets/vertex_core.glsl", "BloxEngine/assets/fragment_core.glsl");
		
		Cube cube(Material::black_rubber, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.5f));
		cube.Initialize();

		Grass cube1(Material::black_rubber, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(1.0f));
		cube1.Initialize();
		Grass cube2(Material::black_rubber, glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(1.0f));
		cube2.Initialize();
		Grass cube3(Material::black_rubber, glm::vec3(2.0f, 0.0f, -1.0f), glm::vec3(1.0f));
		cube3.Initialize();
		Grass cube4(Material::black_rubber, glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(1.0f));
		cube4.Initialize();
		Grass cube5(Material::black_rubber, glm::vec3(-2.0f, 0.0f, -1.0f), glm::vec3(1.0f));
		cube5.Initialize();

		Grass cube11(Material::black_rubber, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f));
		cube11.Initialize();
		Grass cube12(Material::black_rubber, glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(1.0f));
		cube12.Initialize();
		Grass cube13(Material::black_rubber, glm::vec3(2.0f, 0.0f, 1.0f), glm::vec3(1.0f));
		cube13.Initialize();
		Grass cube14(Material::black_rubber, glm::vec3(-1.0f, 0.0f, 1.0f), glm::vec3(1.0f));
		cube14.Initialize();
		Grass cube15(Material::black_rubber, glm::vec3(-2.0f, 0.0f, 1.0f), glm::vec3(1.0f));
		cube15.Initialize();

		Grass cube21(Material::black_rubber, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f));
		cube21.Initialize();
		Grass cube22(Material::black_rubber, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f));
		cube22.Initialize();
		Grass cube23(Material::black_rubber, glm::vec3(2.0f, 0.0f, 0.0f), glm::vec3(1.0f));
		cube23.Initialize();
		Grass cube24(Material::black_rubber, glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(1.0f));
		cube24.Initialize();
		Grass cube25(Material::black_rubber, glm::vec3(-2.0f, 0.0f, 0.0f), glm::vec3(1.0f));
		cube25.Initialize();

		Lamp lamp(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), 1.0f, 0.07f, 0.032f, glm::vec3(-1.5f, 2.5f, -0.5f), glm::vec3(0.25f));
		lamp.Initialize();

		DirLight dirLight = { glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.1f), glm::vec3(0.4f), glm::vec3(0.75f) };

		/*SpotLight sl = {camera.cameraPos, camera.cameraFront,
			glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(20.0f)),
			1.0f, 0.07f, 0.032f,
			glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(1.0f)
		};*/


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

			view = camera.getViewMatrix();
			projection = glm::perspective(glm::radians(camera.zoom), (float)width / (float)height, 0.1f, 100.0f);

			// Rad nad Shader-om
			sh.Activate();
			sh.set3Float("light.position", lamp.pos);
			sh.set3Float("viewPos", camera.cameraPos);

			//sl.position = camera.cameraPos;
			//sl.direction = camera.cameraFront;
			//sl.render(0, sh);
			//sh.setInt("noSpotLights", 1);

			dirLight.render(sh);

			lamp.pointLight.render(0, sh);
			sh.setInt("noPointLights", 1);

			sh.SetMat4("view", view);
			sh.SetMat4("projection", projection);


			cube1.Render(sh);
			cube2.Render(sh);
			cube3.Render(sh);
			cube4.Render(sh);
			cube5.Render(sh);

			cube11.Render(sh);
			cube12.Render(sh);
			cube13.Render(sh);
			cube14.Render(sh);
			cube15.Render(sh);

			cube21.Render(sh);
			cube22.Render(sh);
			cube23.Render(sh);
			cube24.Render(sh);
			cube25.Render(sh);

			cube.Render(sh);
			lamp.Render(sh);

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


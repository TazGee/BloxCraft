#include <iostream>
#include "Renderer.h"

int main()
{
	std::cout << "Starting..." << std::endl;

	// Pozivanje konstruktora klase Renderer
	Renderer rend(800, 600);

	// Main game loop
	while (rend.gameRunning)
	{
		rend.RenderFrame();
	}
	rend.TerminateGame();

	return 0;
}


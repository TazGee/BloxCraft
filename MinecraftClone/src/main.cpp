#include <iostream>
#include "Renderer.h"

int main()
{
	std::cout << "Starting..." << std::endl;
	Renderer rend(800, 600);
	while (rend.gameRunning)
	{
		rend.RenderFrame();
	}
	rend.TerminateGame();
	return 0;
}


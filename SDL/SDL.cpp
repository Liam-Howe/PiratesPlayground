// SDL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define SDL_MAIN_HANDLED

#pragma comment(lib,"SDL2.lib") 
#pragma comment(lib, "SDL2_image.lib")
#pragma comment(lib, "SDL2_net.lib")
#pragma comment(lib, "SDL2_mixer.lib")
#pragma comment(lib, "SDL2_gfx.lib")
#pragma comment(lib, "SDL2_ttf.lib")


#include "Application.h"
int main(int argc, char** argv) 
{
	Application* app = new Application();

	app->initialize("SDL", 0, 0, 1920, 1080, SDL_WINDOW_INPUT_FOCUS);
	app->loop();
	app->cleanUp();

	return 0;
}

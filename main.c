#include <stdbool.h>
#include <stdio.h>

#include <SDL.h>

#define WIDTH 512
#define HEIGHT 384
#define TARGET_SIZE 16

int main(int argc, char *argv[])
{
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;

	// Init SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("SDL_Init error: %s\n", SDL_GetError());
		goto bail;
	}

	// Create display window
	window = SDL_CreateWindow(
		"SDL2 Accelerometer Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		WIDTH, HEIGHT, 0);
	if (window == NULL)
	{
		printf("Failed to create window: %s\n", SDL_GetError());
		goto bail;
	}
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer == NULL)
	{
		printf("Failed to create renderer: %s\n", SDL_GetError());
		goto bail;
	}
	SDL_RenderClear(renderer);
	SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT);

	// Render
	SDL_Rect r = { 0, 0, TARGET_SIZE, TARGET_SIZE };
	if (SDL_SetRenderDrawColor(renderer, 20, 90, 255, 255) != 0)
	{
		printf("Failed to draw color: %s\n", SDL_GetError());
		goto bail;
	}
	if (SDL_RenderFillRect(renderer, &r) != 0)
	{
		printf("Failed to draw target: %s\n", SDL_GetError());
		goto bail;
	}

	SDL_RenderPresent(renderer);

	bool quit = false;
	while (!quit)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
				case SDL_QUIT:
					quit = true;
					break;
				default:
					break;
			}
		}
		SDL_Delay(100);
	}

bail:
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

#include <stdbool.h>
#include <stdio.h>

#include <SDL.h>

#define WIDTH 512
#define HEIGHT 384
#define TARGET_SIZE 16
#define WIDTH_T (WIDTH - TARGET_SIZE)
#define HEIGHT_T (HEIGHT - TARGET_SIZE)
SDL_Color axisColors[] = {
	{ 255, 20, 20, 255 }, { 20, 255, 20, 255 }, { 20, 20, 255, 255 },
	{ 255, 255, 20, 255 }, { 255, 20, 255, 255 }, { 255, 20, 255, 255 },
};

int main(int argc, char *argv[])
{
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;

	// Init SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0)
	{
		printf("SDL_Init error: %s\n", SDL_GetError());
		goto bail;
	}
	if (!SDL_SetHint(SDL_HINT_ACCELEROMETER_AS_JOYSTICK, "1"))
	{
		printf("Cannot set accelerometer hint: %s\n", SDL_GetError());
		goto bail;
	}
	if (SDL_NumJoysticks() == 0)
	{
		printf("No joystics detected");
		goto bail;
	}
	SDL_GameController *gc = SDL_GameControllerOpen(0);
	SDL_Joystick *j = SDL_GameControllerGetJoystick(gc);
	printf("Joystick 0 has %d axes\n", SDL_JoystickNumAxes(j));

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
	SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT);

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

		if (SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255) != 0)
		{
			printf("Failed to draw color: %s\n", SDL_GetError());
			goto bail;
		}
		SDL_RenderClear(renderer);
		for (int i = 0; i < SDL_JoystickNumAxes(j); i += 2)
		{
			const Sint16 axis1 = SDL_JoystickGetAxis(j, i);
			const Sint16 axis2 = SDL_JoystickGetAxis(j, i + 1);
			const SDL_Color c = axisColors[i / 2];
			if (SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a) != 0)
			{
				printf("Failed to draw color: %s\n", SDL_GetError());
				goto bail;
			}
			SDL_Rect r = {
				axis1 * WIDTH_T / 2 / 32768 + WIDTH_T / 2,
				axis2 * HEIGHT_T / 2 / 32768 + HEIGHT_T / 2,
				TARGET_SIZE, TARGET_SIZE
			};
			if (SDL_RenderFillRect(renderer, &r) != 0)
			{
				printf("Failed to draw target: %s\n", SDL_GetError());
				goto bail;
			}
		}
		SDL_RenderPresent(renderer);
		SDL_Delay(10);
	}

bail:
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

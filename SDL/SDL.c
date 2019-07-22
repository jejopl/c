#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>

typedef struct {
	int x, y;
	short life;
	char* name;
} Man;

typedef struct {

	// Players
	Man man;

	// Images
	SDL_Texture *star;

} GameState;

int processEvents(SDL_Window *window, GameState *game) {
	SDL_Event event;
	int done = 0;
	// Check for events
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_WINDOWEVENT_CLOSE: {
			if (window) {
				SDL_DestroyWindow(window);
				window = NULL;
				done = 1;
			}
		}
		break;

		// If key is pressed, do something
		case SDL_KEYDOWN: {
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				done = 1;
				break;
			}
		}
						  break;
		case SDL_QUIT:
			// quit out of the game
			done = 1;
			break;
		}
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_LEFT]) {
		game->man.x -= 5;
	}
	if (state[SDL_SCANCODE_RIGHT]) {
		game->man.x += 5;
	}
	if (state[SDL_SCANCODE_UP]) {
		game->man.y -= 5;
	}
	if (state[SDL_SCANCODE_DOWN]) {
		game->man.y += 5;
	}

	return done;
}

void doRender(SDL_Renderer* renderer, GameState *game) {
	// Set the drawing color to blue
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

	// Clear the screen (to blue)
	SDL_RenderClear(renderer);

	// Set drawing color to white
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	SDL_Rect rect = { game->man.x, game->man.y, 200, 200 };
	SDL_RenderFillRect(renderer, &rect);

	// We are done drawing, "present" or show to the screen what've drawn
	SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[])

{
	GameState gameState;
	SDL_Window* window = NULL;     // Declare a window
	SDL_Renderer* renderer = NULL;     // Declare a renderer
	SDL_Surface* starSurface = NULL;

	SDL_Init(SDL_INIT_VIDEO);   // Initialize SDL2

	gameState.man.x = 320 - 40;
	gameState.man.y = 240 - 40;


	// Create an application window with the following settings:
	window = SDL_CreateWindow("Game Window",    // Window title
		SDL_WINDOWPOS_UNDEFINED,  // Initial x position
		SDL_WINDOWPOS_UNDEFINED,  // Initial y position
		640,  // Width, in pixe
		480,  // Height, in pixels
		0     // flags
	);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	starSurface = IMG_Load("star.png");
	if(starSurface == NULL) {
		printf("Cannot find star.png!\n\n");
		SDL_Quit();
		return 1;
	}

	gameState.star = SDL_CreateTextureFromSurface(renderer, starSurface);
	SDL_FreeSurface(starSurface);

	int done = 0;

	// Event loop
	while (!done) {

		// Check for the events
		done = processEvents(window, &gameState);

		// Render
		doRender(renderer, &gameState);

		// Don't burn the CPU
		// SDL_Delay(10);
	}

	// Shitdown game and unload all memory
	SDL_DestroyTexture(gameState.star);

	// Close and destroy the window
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	// Clean up
	SDL_Quit();

	return 0;
}
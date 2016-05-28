#include <SDL2/SDL.h>

#include "Chip8.h"

#undef main

Chip8 chip8;

SDL_Event event;

SDL_Window *mainWindow;
SDL_Renderer *renderer;
SDL_Surface *pixelBlackSurface, *pixelWhiteSurface;
SDL_Texture *pixelBlack, *pixelWhite;
SDL_Rect pixels[GFX_X * GFX_Y];

int main(int argc, char const *argv[])
{
	chip8.init();
	chip8.loadProgram("C:\\Users\\Arnav\\Desktop\\pong2.c8");

	mainWindow = SDL_CreateWindow("Hello World!", 100, 100, 640, 320, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	pixelBlackSurface = SDL_LoadBMP("PixelBlack.bmp");
	pixelWhiteSurface = SDL_LoadBMP("PixelWhite.bmp");

	pixelBlack = SDL_CreateTextureFromSurface(renderer, pixelBlackSurface);
	pixelWhite = SDL_CreateTextureFromSurface(renderer, pixelWhiteSurface);

	for (int i = 0; i < GFX_Y; ++i) {
		for (int j = 0; j < GFX_X; ++j) {
			SDL_Rect pixel;
			pixel.x = 10 * j;
			pixel.y = 10 * i;
			pixel.h = 10;
			pixel.w = 10;
			pixels[i * GFX_X + j] = pixel;
		}
	}

	while(1) {
		if(SDL_PollEvent(&event)) {
			if(event.type == SDL_QUIT) {
				return 0;
			}
		}
		chip8.runCycle();
		if(chip8.drawFlag) {
			chip8.drawFlag = false;
			SDL_RenderClear(renderer);
			for (int i = 0; i < sizeof(chip8.gfx)/sizeof(chip8.gfx[0]); ++i) {
				if(chip8.gfx[i] == 1) {
					SDL_RenderCopy(renderer, pixelWhite, NULL, &(pixels[i]));
				} else {
					SDL_RenderCopy(renderer, pixelBlack, NULL, &(pixels[i]));
				}
			}
			SDL_RenderPresent(renderer);
		}
		SDL_Delay(17);
	}
	return 0;
}
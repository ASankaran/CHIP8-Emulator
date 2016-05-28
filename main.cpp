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
	chip8.loadProgram("C:\\Users\\Arnav\\Desktop\\tetris.c8");

	SDL_Init(SDL_INIT_VIDEO);

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
			switch(event.type) {
				case SDL_QUIT:
					return 0;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym) {
						case SDLK_1:
							chip8.key[0x1] = 1;
							break;
						case SDLK_2:
							chip8.key[0x2] = 1;
							break;
						case SDLK_3:
							chip8.key[0x3] = 1;
							break;
						case SDLK_4:
							chip8.key[0xC] = 1;
							break;
						case SDLK_q:
							chip8.key[0x4] = 1;
							break;
						case SDLK_w:
							chip8.key[0x5] = 1;
							break;
						case SDLK_e:
							chip8.key[0x6] = 1;
							break;
						case SDLK_r:
							chip8.key[0xD] = 1;
							break;
						case SDLK_a:
							chip8.key[0x7] = 1;
							break;
						case SDLK_s:
							chip8.key[0x8] = 1;
							break;
						case SDLK_d:
							chip8.key[0x9] = 1;
							break;
						case SDLK_f:
							chip8.key[0xE] = 1;
							break;
						case SDLK_z:
							chip8.key[0xA] = 1;
							break;
						case SDLK_x:
							chip8.key[0x0] = 1;
							break;
						case SDLK_c:
							chip8.key[0xB] = 1;
							break;
						case SDLK_v:
							chip8.key[0xF] = 1;
							break;
					}
					break;
				case SDL_KEYUP:
					switch(event.key.keysym.sym) {
						case SDLK_1:
							chip8.key[0x1] = 0;
							break;
						case SDLK_2:
							chip8.key[0x2] = 0;
							break;
						case SDLK_3:
							chip8.key[0x3] = 0;
							break;
						case SDLK_4:
							chip8.key[0xC] = 0;
							break;
						case SDLK_q:
							chip8.key[0x4] = 0;
							break;
						case SDLK_w:
							chip8.key[0x5] = 0;
							break;
						case SDLK_e:
							chip8.key[0x6] = 0;
							break;
						case SDLK_r:
							chip8.key[0xD] = 0;
							break;
						case SDLK_a:
							chip8.key[0x7] = 0;
							break;
						case SDLK_s:
							chip8.key[0x8] = 0;
							break;
						case SDLK_d:
							chip8.key[0x9] = 0;
							break;
						case SDLK_f:
							chip8.key[0xE] = 0;
							break;
						case SDLK_z:
							chip8.key[0xA] = 0;
							break;
						case SDLK_x:
							chip8.key[0x0] = 0;
							break;
						case SDLK_c:
							chip8.key[0xB] = 0;
							break;
						case SDLK_v:
							chip8.key[0xF] = 0;
							break;
					}
					break;
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
		SDL_Delay(10);
	}
	return 0;
}
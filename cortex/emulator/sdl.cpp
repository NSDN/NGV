#include "sdl.h"

#include "./Drivers/SDL2/Include/SDL.h"
#include "./Drivers/SDL2/Include/SDL_mixer.h"

#include <string>

#include <cstdlib>
#include <cstdio>
#include <ctime>

void processEvent();
SDL_Event event;
SDL_Renderer* renderer;
SDL_Rect rect, pixelRect;

SDL_Window* window;
SDL_Surface* icon;
SDL_Surface* surface;

const uint16_t LCD_WIDTH = 854, LCD_HEIGHT = 480;
static uint16_t LCD_SCALE = 1;

static uint32_t LCD_PTR;
static uint8_t LCD_SPD = 0;

extern uint8_t initFlag, endFlag, reset;
extern uint16_t keyValue;

#include <setjmp.h>
extern jmp_buf rstPos;

static uint8_t _progress = 0;
static uint16_t _count = 0;
void progress() {
    char c;
    if (_count < 100) { _count += 1; return; }
    else {
        _count = 0;

        switch (_progress) {
            case 0: c = '-';    _progress += 1; break;
            case 1: c = '\\';   _progress += 1; break;
            case 2: c = '|';    _progress += 1; break;
            case 3: c = '/';    _progress  = 0; break;
        }
        printf("%c\b", c);
    }
}

void delay(uint32_t ms) { 
    clock_t begin = clock();
    while ((uint32_t) ((double) (clock() - begin) / CLK_TCK * 1000) < ms)
        processEvent();
}

void pixel(SDL_Renderer* render, uint16_t x, uint16_t y) {
	pixelRect.x = x * pixelRect.w;
	pixelRect.y = y * pixelRect.h;
	SDL_RenderFillRect(renderer, &pixelRect);
}

void color(uint32_t color) {
    SDL_SetRenderDrawColor(renderer,
            (color >> 16) & 0xFF, 
            (color >> 8 ) & 0xFF,
            (color >> 0 ) & 0xFF,
            0xFF
        );
}

void pos(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    rect.x = x1; rect.y = y1;
	rect.w = x2 - x1 + 1; rect.h = y2 - y1 + 1;
	LCD_PTR = 0;
}

void write(uint32_t data) {
    if (LCD_PTR < rect.w * rect.h) {
        color(data);
        pixel(renderer, rect.x + LCD_PTR % rect.w, rect.y + LCD_PTR / rect.w);
        LCD_PTR += 1;
    } else LCD_PTR = 0;
    SDL_RenderPresent(renderer);
}

void writes(uint32_t* data, uint32_t len) {
    while (LCD_PTR < rect.w * rect.h) {
        color(data[LCD_PTR]);
        pixel(renderer, rect.x + LCD_PTR % rect.w, rect.y + LCD_PTR / rect.w);
        LCD_PTR += 1;
        if (LCD_PTR >= len) break;
    }
    SDL_RenderPresent(renderer);
}

void flash(uint32_t data, uint32_t n) {
    color(data);
    while (LCD_PTR < rect.w * rect.h) {
        pixel(renderer, rect.x + LCD_PTR % rect.w, rect.y + LCD_PTR / rect.w);
        LCD_PTR += 1;
        if (LCD_PTR >= n) break;
    }
    SDL_RenderPresent(renderer);
}

void* loadBGM(std::string path) {
    std::string url = "Assets/" + path;
    return (void*) Mix_LoadMUS(url.c_str());
}

void unloadBGM(void* bgm) {
    Mix_FreeMusic((Mix_Music*) bgm);
}

void stopBGM() {
    Mix_PauseMusic();
}

void playBGM(void* bgm) {
    Mix_PlayMusic((Mix_Music*) bgm, -1);
}

void initSDL() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	window = SDL_CreateWindow(
		"ngv-emu",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		LCD_WIDTH, LCD_HEIGHT, SDL_WINDOW_SHOWN
	);
    
	surface = SDL_GetWindowSurface(window);
	icon = SDL_LoadBMP("Assets/logo.bmp");
	if (icon != 0) SDL_SetWindowIcon(window, icon);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    Mix_Init(
        MIX_INIT_MP3 | MIX_INIT_FLAC | MIX_INIT_MID |
        MIX_INIT_OGG | MIX_INIT_OPUS | MIX_INIT_MOD
    );
    Mix_OpenAudio(44100, AUDIO_F32SYS, 2, 4096);
}

void deinitSDL() {
    SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
    Mix_CloseAudio();
    Mix_Quit();
	SDL_Quit();
}

void resizeWindow(uint16_t scale) {
    LCD_SCALE = scale;
    SDL_Rect monitor;
    SDL_GetDisplayBounds(0, &monitor);
    int width = LCD_WIDTH * LCD_SCALE, height = LCD_HEIGHT * LCD_SCALE;
    SDL_SetWindowSize(window, width, height);
    SDL_SetWindowPosition(window, (monitor.w - width) / 2, (monitor.h - height) / 2);
	pixelRect.w = LCD_SCALE;
	pixelRect.h = LCD_SCALE;
}

#include "util.h"

void processEvent() {
    while (SDL_PollEvent(&event) != 0) {
		if (event.type == SDL_QUIT) {
			if (initFlag == 0) {
                deinitSDL();
                exit(0);
            } else endFlag = 1;
		}
		if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    if (initFlag == 0) {
                        deinitSDL();
                        exit(0);
                    } else endFlag = 1;
                    break;
                case SDLK_BACKSPACE:
                    if (initFlag == 0)
                        longjmp(rstPos, 0);
                    else reset = 1;
                    break;

                case SDLK_w:        keyValue |= LPAD_UP;    break;
                case SDLK_s:        keyValue |= LPAD_DOWN;  break;
                case SDLK_a:        keyValue |= LPAD_LEFT;  break;
                case SDLK_d:        keyValue |= LPAD_RIGHT; break;
                case SDLK_UP:       keyValue |= RPAD_UP;    break;
                case SDLK_DOWN:     keyValue |= RPAD_DOWN;  break;
                case SDLK_LEFT:     keyValue |= RPAD_LEFT;  break;
                case SDLK_RIGHT:    keyValue |= RPAD_RIGHT; break;

                case SDLK_F1:       keyValue |= KEY_F1;     break;
                case SDLK_F2:       keyValue |= KEY_F2;     break;
                case SDLK_F3:       keyValue |= KEY_F3;     break;
                case SDLK_F4:       keyValue |= KEY_F4;     break;
                case SDLK_F5:       keyValue |= KEY_F5;     break;
                case SDLK_F6:       keyValue |= KEY_F6;     break;
                case SDLK_F7:       keyValue |= KEY_F7;     break;
                case SDLK_F8:       keyValue |= KEY_F8;     break;

                default:
                    break;
			}
		}
		if (event.type == SDL_KEYUP) {
			switch (event.key.keysym.sym) {
                case SDLK_w:        keyValue &= ~LPAD_UP;    break;
                case SDLK_s:        keyValue &= ~LPAD_DOWN;  break;
                case SDLK_a:        keyValue &= ~LPAD_LEFT;  break;
                case SDLK_d:        keyValue &= ~LPAD_RIGHT; break;
                case SDLK_UP:       keyValue &= ~RPAD_UP;    break;
                case SDLK_DOWN:     keyValue &= ~RPAD_DOWN;  break;
                case SDLK_LEFT:     keyValue &= ~RPAD_LEFT;  break;
                case SDLK_RIGHT:    keyValue &= ~RPAD_RIGHT; break;

                case SDLK_F1:       keyValue &= ~KEY_F1;     break;
                case SDLK_F2:       keyValue &= ~KEY_F2;     break;
                case SDLK_F3:       keyValue &= ~KEY_F3;     break;
                case SDLK_F4:       keyValue &= ~KEY_F4;     break;
                case SDLK_F5:       keyValue &= ~KEY_F5;     break;
                case SDLK_F6:       keyValue &= ~KEY_F6;     break;
                case SDLK_F7:       keyValue &= ~KEY_F7;     break;
                case SDLK_F8:       keyValue &= ~KEY_F8;     break;

                default:
                    break;
			}
		}
	}
}

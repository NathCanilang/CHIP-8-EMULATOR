#ifndef PLATFORM_H
#define PLATFORM_H

#include "SDL3/SDL.h"
#include "SDL3/SDL_main.h"

typedef struct Platform{
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
} platform_t;

void init_platform(platform_t* platform, const char* title, int win_width, int win_height, int texture_width, int texture_height);
void destroy_platfrom(platform_t* platform);
void update(platform_t* platform, void const* buffer, int pitch);
bool process_user_input(uint8_t* keys);

#endif
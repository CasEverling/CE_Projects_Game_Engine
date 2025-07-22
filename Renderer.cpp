#include <SDL3/SDL.h>
#include "Renderer.h"

SDL_Window* Renderer::window = NULL;
SDL_Renderer* Renderer::renderer = NULL;
float Renderer::lightness = 0;
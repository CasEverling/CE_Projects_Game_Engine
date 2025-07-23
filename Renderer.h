#ifndef RENDERER_H
#define RENDERER_H

#include <SDL3/SDL.h>

class Renderer 
{
    public:
        static SDL_Window *window;
        static SDL_Renderer *renderer;
        static float lightness;
};

#endif
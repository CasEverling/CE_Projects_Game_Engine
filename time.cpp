#include <SDL3/SDL.h>
#include "time.h"

#define INITIAL_DELTATIME 1
Uint32 Time::previousTime = 0;

float Time::deltatime = INITIAL_DELTATIME;

void Time::Tick() {
    Uint32 currentTime = SDL_GetTicks();
    deltatime = (currentTime - previousTime) / 1000.0f;
    previousTime = currentTime;
}


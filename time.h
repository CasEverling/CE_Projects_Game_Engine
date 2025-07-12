#include <SDL3/SDL.h>

class Time 
{
    private:
        static Uint32 previousTime;
    public:
        static float deltatime;
        static void Tick();
};
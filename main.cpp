/* textures.c ... */

/*
 * This example creates an SDL window and renderer, and then draws some
 * textures to it every frame.
 *
 * This code is public domain. Feel free to use it for any purpose!
 */

#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */

#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>

#include <SDL3/SDL_main.h>
#include "Renderer.h"
#include "resource_manager.h"
#include "time.h"
#include "character.h"
#include "sprite_renderer.h"
#include "input_map.h"
#include "input_manager.h"
#include "block.h"
#include "collider_3d.h"


/* We will use this renderer to draw into this window every frame. */
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 800
#define TEXTURE_NAME "astronaut"

static float texture_width, texture_height;

static float position = 0;

void DebugRenderFPS()
{
    if (!Renderer::renderer) return;

    float fps = (Time::deltatime > 0.0f) ? (1.0f / Time::deltatime) : 0.0f;

    char buffer[64];
    std::snprintf(buffer, sizeof(buffer), "FPS: %.1f", fps);

    SDL_SetRenderDrawColor(Renderer::renderer, 255, 255, 255, SDL_ALPHA_OPAQUE); // White
    SDL_RenderDebugText(Renderer::renderer, 10, 10, buffer);
}

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_SetAppMetadata("Example Renderer Textures", "1.0", "com.example.renderer-textures");

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMEPAD | SDL_INIT_EVENTS)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("examples/renderer/textures", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &Renderer::window, &Renderer::renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_Texture* texture = ResourceManager::get_texture(TEXTURE_NAME);
    if (!texture) {
        SDL_Log("Failed to load texture: %s", TEXTURE_NAME);
        return SDL_APP_FAILURE;
    }
    if (!SDL_GetTextureSize(texture, &texture_width, &texture_height)) {
        SDL_Log("Failed to get texture size: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (SDL_AddGamepadMappingsFromFile("gamecontrollerdb.txt") < 0) {
    SDL_Log("Failed to load mappings: %s", SDL_GetError());
    }

    SDL_SetWindowFullscreen(Renderer::window, true);

    int count = 0;
    SDL_JoystickID *ids = SDL_GetGamepads(&count);   // ✔ correct order & types
    SDL_Log("Number of joysticks: %d", count);
    SDL_free(ids);   // SDL allocates; you free.


    InputMap* inputMap = new InputMap("inputMaps/character_controller_1.csv");
    InputManager::activateMap(inputMap);

    new Character(0,0,7,7);
    
    

    for (int i = 9; i < 10; i ++)
        for (int j = 0; j < 10; j++)
            new Block(j,i,0);
    
    for (int i = 1; i < 9; i ++)
        for (int j = 0; j < 1; j++)
            new Block(j,i,0);
    
    for (int i = 1; i < 9; i ++)
        for (int j = 9; j < 10; j++)
            new Block(j,i,0);
    
    for (int i = 3; i < 5; i ++)
        for (int j = 3; j < 5; j++)
            new Block(j,i,0);

    
    //new Character(20,0, - 100, 10);

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    return InputManager::AnalyseInputs(event);
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    Time::Tick();

    Renderer::lightness = 0;
    

    /* as you can see from this, rendering draws over whatever was drawn before it. */
    SDL_SetRenderDrawColor(Renderer::renderer, 50*Renderer::lightness, 50*Renderer::lightness, 50*Renderer::lightness, SDL_ALPHA_OPAQUE);  /* black, full alpha */
    SDL_RenderClear(Renderer::renderer);  /* start with a blank canvas. */

    /* Components that have to be visisted every frame */
    InputManager::AnalyseAxis();
    UpdatableObject::UpdateAll();
    Collider3D::CheckAllCollisions();
    SpriteRenderer::displayAll();


    DebugRenderFPS();
    SDL_RenderPresent(Renderer::renderer);  /* put it all on the screen! */

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    ResourceManager::cleanup();
    UpdatableObject::cleanup();
    SpriteRenderer::cleanUp();
    EventManager::cleanup();
    Collider3D::cleanup();
}
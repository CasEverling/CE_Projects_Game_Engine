#include "input_manager.h"
#include "input_map.h"
#include <SDL3/SDL.h>
#include "event_manager.h"
#include <list>
#include <unordered_map>

std::list<InputMap*> InputManager::_activeInputMaps = std::list<InputMap*>();
std::unordered_map<int, bool> InputManager::previousState = std::unordered_map<int, bool>();


SDL_AppResult InputManager::AnalyseInputs(SDL_Event *event) {
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }

    if (!_activeInputMaps.size()) return SDL_APP_CONTINUE;

    if (event->type == SDL_EVENT_KEY_DOWN) {
        SDL_Keycode key_code = event->key.key;
        if (previousState[key_code]) return SDL_APP_CONTINUE;
        previousState[key_code] = true;
        SDL_Log("KeyDown: %d", key_code);
        for (InputMap* inputMap : _activeInputMaps) {
            if (inputMap->keyDownEvents.find(key_code) != inputMap->keyDownEvents.end())
                EventManager::Invoke(inputMap->keyDownEvents[key_code]);
        }
        return SDL_APP_CONTINUE;
    } 

    if (event->type == SDL_EVENT_KEY_UP) {
        SDL_Keycode key_code = event->key.key;
        SDL_Log("KeyUp: %d", key_code);
        previousState[key_code] = false;
        for (InputMap* inputMap : _activeInputMaps) {
            if (inputMap->keyUpEvents.find(key_code) != inputMap->keyUpEvents.end())
                EventManager::Invoke(inputMap->keyUpEvents[key_code]);
        }
        return SDL_APP_CONTINUE;
    }

    if (event->type == SDL_EVENT_GAMEPAD_ADDED) {
        SDL_Log("Controller connected: %u", (unsigned int) event->jdevice.which);
    }

    if (event->type == SDL_EVENT_GAMEPAD_BUTTON_DOWN) {
        SDL_Log("Gamepad %u pressed", (unsigned int) event->gbutton.button);
    }

    if (event->type == SDL_EVENT_JOYSTICK_BUTTON_DOWN) {
        SDL_Log("Gamepad %u pressed", (unsigned int) event->jbutton.button);
    }

    if (event->type == SDL_EVENT_JOYSTICK_REMOVED) {
        SDL_Log("Joystick Removed");
    }
    
    return SDL_APP_CONTINUE;

}


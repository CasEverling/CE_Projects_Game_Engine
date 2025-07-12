#ifndef INPUT_MAP_H
#define INPUT_MAP_H

#include <SDL3/SDL.h>
#include <unordered_map>
#include <string>


class InputMap {
    private:
        bool is_active();
    public:
        std::unordered_map<SDL_Keycode, std::string> keyDownEvents;
        std::unordered_map<SDL_Keycode, std::string> keyUpEvents;
    
        //std::unordered_map<int, std::string> JoystickButtonDownEvents;
        //std::unordered_map<int, std::string> JoystickButtonUpEvents;
        //std::unordered_map<int, std::string> JoystickGetJoystickAxis;

        InputMap(std::string csv_path);
        ~InputMap();
};

#endif
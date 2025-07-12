#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "input_map.h"
#include <list>
#include <unordered_map>

class InputManager {
    private:
        static std::list<InputMap*> _activeInputMaps;
        static std::unordered_map<int, bool> previousState;
        
    public:
        static void activateMap(InputMap* inputMap) {
            _activeInputMaps.push_back(inputMap);
        };

        static void deactivateMap(InputMap* inputMap) {
            _activeInputMaps.remove(inputMap);
        };

        static SDL_AppResult AnalyseInputs(SDL_Event *event);
        static void AnalyseAxis() {};
};

#endif
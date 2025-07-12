#include "input_map.h"
#include <SDL3/SDL.h>
#include <string>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>


InputMap::InputMap(std::string csv_path) {
    std::ifstream file(csv_path.c_str());

    if (!file.is_open()) {
        SDL_Log("Unable to open Input Map at location %s", csv_path.c_str());
        return;
    }

    std::string line;
    std::vector<std::string> row;
    while (std::getline(file, line)) {
        row.clear();
        std::stringstream ss(line);
        std::string cell;
        while (std::getline(ss, cell, ',')) {
            row.push_back(cell);
        }

        switch (row[0][0]) {
        case '0':
            SDL_Log("%d %s", SDL_GetKeyFromName(row[1].c_str()), row[2].c_str());
            keyUpEvents[SDL_GetKeyFromName(row[1].c_str())] = row[2];
            break;
        case '1':
            keyDownEvents[SDL_GetKeyFromName(row[1].c_str())] = row[2];
            break;
        default:
            SDL_Log("ERROR ON ASSIGNING KEY MAP");
            break;
        }
    }

    file.close();
}

InputMap::~InputMap() {
    InputMap::keyDownEvents.clear();
    InputMap::keyUpEvents.clear();
}
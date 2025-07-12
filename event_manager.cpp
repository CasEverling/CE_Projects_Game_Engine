#include <stdlib.h>
#include <string>
#include <functional>
#include "event_manager.h"
#include <list>
#include <SDL3/SDL.h>
#include <unordered_map>

template <typename T>
using EventMap = std::unordered_map<std::string, std::list<FunctionNode<T>>>;

void EventManager::cleanup() {
    callbacks_void.clear();
    callbacks_int_int.clear();
    callbacks_int.clear();
    callbacks_float_float.clear();
    callbacks_float.clear();
}

/*********************************************************************************************************/
#pragma region void
EventMap<void()> EventManager::callbacks_void = EventMap<void()>();

void EventManager::Invoke(const std::string& event_name) {
    SDL_Log(event_name.c_str());
    if (callbacks_void.find(event_name) != callbacks_void.end())
        for (auto& callback : callbacks_void[event_name])
            callback.callback();
    else SDL_Log("Event %s of type (void) does not exist", event_name.c_str());
}

cbId EventManager::AddListener(const std::string& event_name, std::function<void()> listener) {
    size_t callbackId;

    // Defining the callbackId
    if (callbacks_void[event_name].empty()) callbackId = 1;
    else callbackId = callbacks_void[event_name].back().callbackId + 1;
    
    // Adding to list
    callbacks_void[event_name].push_back(FunctionNode<void()> {callbackId, listener});

    // Returning value
    return callbackId;
}

void EventManager::RemoveListener(const std::string& event_name, std::function<void()> /*Unused, jut defines which overload to use*/, cbId callbackId) {
    if (callbacks_void.find(event_name) != callbacks_void.end()) {
        for (auto callback = callbacks_void[event_name].begin(); callback != callbacks_void[event_name].end(); callback++) {
            if (callback->callbackId == callbackId) {
                callbacks_void[event_name].erase(callback);
                if (callbacks_void[event_name].size() == 0) {
                    callbacks_void.erase(event_name.c_str());
                    return;
                }
            }
        }
        SDL_Log("Event %s of type (void) does not exist", event_name.c_str());
    } else SDL_Log("Event %s of type (void) does not exist", event_name.c_str());
}

#pragma endregion

/*********************************************************************************************************/
#pragma region (int)

EventMap<void(int)> EventManager::callbacks_int = EventMap<void(int)>();

void EventManager::Invoke(const std::string& event_name, int x) {
    if (callbacks_int.find(event_name) != callbacks_int.end())
        for (auto& callback : callbacks_int[event_name])
            callback.callback(x);
    else SDL_Log("Event %s of type (int) does not exist", event_name.c_str());
}

cbId EventManager::AddListener(const std::string& event_name, std::function<void(int)> listener) {
    size_t callbackId;

    // Defining the callbackId
    if (callbacks_int[event_name].empty()) callbackId = 1;
    else callbackId = callbacks_int[event_name].back().callbackId + 1;
    
    // Adding to list
    callbacks_int[event_name].push_back(FunctionNode<void(int)> {callbackId, listener});

    // Returning value
    return callbackId;
}

void EventManager::RemoveListener(const std::string& event_name, std::function<void(int)> /*Unused, jut defines which overload to use*/, cbId callbackId) {
    if (callbacks_int.find(event_name) != callbacks_int.end()) {
        for (auto callback = callbacks_int[event_name].begin(); callback != callbacks_int[event_name].end(); callback++) {
            if (callback->callbackId == callbackId) {
                callbacks_int[event_name].erase(callback);
                if (callbacks_int[event_name].size() == 0) {
                    callbacks_int.erase(event_name);
                    return;
                }
            }
        }
        SDL_Log("Event %s of type (int) does not exist", event_name.c_str());
    } else SDL_Log("Event %s of type (int) does not exist", event_name.c_str());
}
#pragma endregion

#pragma region (int,int)

EventMap<void(int,int)> EventManager::callbacks_int_int = EventMap<void(int,int)>();

void EventManager::Invoke(const std::string& event_name, int x, int y) {
    if (callbacks_int_int.find(event_name) != callbacks_int_int.end())
        for (auto& callback : callbacks_int_int[event_name])
            callback.callback(x,y);
    else SDL_Log("Event %s of type (int, int) does not exist", event_name.c_str());
}

cbId EventManager::AddListener(const std::string& event_name, std::function<void(int,int)> listener) {
    size_t callbackId;

    // Defining the callbackId
    if (callbacks_int_int[event_name].empty()) callbackId = 1;
    else callbackId = callbacks_int_int[event_name].back().callbackId + 1;
    
    // Adding to list
    callbacks_int_int[event_name].push_back(FunctionNode<void(int,int)> {callbackId, listener});

    // Returning value
    return callbackId;
}

void EventManager::RemoveListener(const std::string& event_name, std::function<void(int,int)> /*Unused, jut defines which overload to use*/, cbId callbackId) {
    if (callbacks_int_int.find(event_name) != callbacks_int_int.end()) {
        for (auto callback = callbacks_int_int[event_name].begin(); callback != callbacks_int_int[event_name].end(); callback++) {
            if (callback->callbackId == callbackId) {
                callbacks_int_int[event_name].erase(callback);
                if (callbacks_int_int[event_name].size() == 0) {
                    callbacks_int_int.erase(event_name);
                    return;
                }
            }
        }
        SDL_Log("Event %s of type (int, int) does not exist", event_name.c_str());
    } else SDL_Log("Event %s of type (int, int) does not exist", event_name.c_str());
}
#pragma endregion

 
/*********************************************************************************************************/
#pragma region (float) 

EventMap<void(float)> EventManager::callbacks_float = EventMap<void(float)>();

void EventManager::Invoke(const std::string& event_name, float x) {
    if (callbacks_float.find(event_name) != callbacks_float.end())
        for (auto& callback : callbacks_float[event_name])
            callback.callback(x);
    else SDL_Log("Event %s of type (int) does not exist", event_name.c_str());
}

cbId EventManager::AddListener(const std::string& event_name, std::function<void(float)> listener) {
    size_t callbackId;

    // Defining the callbackId
    if (callbacks_float[event_name].empty()) callbackId = 1;
    else callbackId = callbacks_float[event_name].back().callbackId + 1;
    
    // Adding to list
    callbacks_float[event_name].push_back(FunctionNode<void(float)> {callbackId, listener});

    // Returning value
    return callbackId;
}

void EventManager::RemoveListener(const std::string& event_name, std::function<void(float)> /*Unused, jut defines which overload to use*/, cbId callbackId) {
    if (callbacks_float.find(event_name) != callbacks_float.end()) {
        for (auto callback = callbacks_float[event_name].begin(); callback != callbacks_float[event_name].end(); callback++) {
            if (callback->callbackId == callbackId) {
                callbacks_float[event_name].erase(callback);
                if (callbacks_float[event_name].size() == 0) {
                    callbacks_float.erase(event_name);
                    return;
                }
            }
        }
        SDL_Log("Event %s of type (float) does not exist", event_name.c_str());
    } else SDL_Log("Event %s of type (float) does not exist", event_name.c_str());
}

#pragma endregion

/*********************************************************************************************************/
#pragma region (float, float)

EventMap<void(float, float)> EventManager::callbacks_float_float = EventMap<void(float, float)>();

void EventManager::Invoke(const std::string& event_name, float x, float y) {
    if (callbacks_float_float.find(event_name) != callbacks_float_float.end())
        for (auto& callback : callbacks_float_float[event_name])
            callback.callback(x, y);
    else SDL_Log("Event %s of type (int) does not exist", event_name.c_str());
}

cbId EventManager::AddListener(const std::string& event_name, std::function<void(float, float)> listener) {
    size_t callbackId;

    // Defining the callbackId
    if (callbacks_float_float[event_name].empty()) callbackId = 1;
    else callbackId = callbacks_float_float[event_name].back().callbackId + 1;
    
    // Adding to list
    callbacks_float_float[event_name].push_back(FunctionNode<void(float, float)> {callbackId, listener});

    // Returning value
    return callbackId;
}

void EventManager::RemoveListener(const std::string& event_name, std::function<void(float, float)> /*Unused, jut defines which overload to use*/, cbId callbackId) {
    if (callbacks_float_float.find(event_name) != callbacks_float_float.end()) {
        for (auto callback = callbacks_float_float[event_name].begin(); callback != callbacks_float_float[event_name].end(); callback++) {
            if (callback->callbackId == callbackId) {
                callbacks_float_float[event_name].erase(callback);
                if (callbacks_float_float[event_name].size() == 0) {
                    callbacks_float_float.erase(event_name);
                    return;
                }
            }
        }
        SDL_Log("Event %s of type (float, float) does not exist", event_name.c_str());
    } else SDL_Log("Event %s of type (float, float) does not exist", event_name.c_str());
}

#pragma endregion

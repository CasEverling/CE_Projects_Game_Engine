#include "interanal_event_manager.h"
#include <unordered_map>
#include <memory>
#include <vector>
#include <string>
#include <functional>
#include <SDL3/SDL.h>

template <typename ...Args>
void InternalEventManager::invoke(std::string&& eventName, Args&&... args) noexcept {
    try {
        for (auto f : this->getEventMap(std::function<void(Args...)> {})[std::forward<std::string>(eventName)])
            f(std::forward<Args>(args)...);
    } catch (EventManagerExceptions exceptionCode) {
        if (exceptionCode == EventManagerExceptions::UnsusportedEventType) {
            SDL_Log("Event %s invoked with paramenters not allows", eventName.c_str());
        }
    } catch (...) {
        SDL_Log("Unhandled exception on invoking %s", eventName.c_str());
    }
}

template <typename T>
void InternalEventManager::addListener(std::string&& eventName, std::function<T>&& function) noexcept {
    try {
        this->getEventMap(std::function<T> {})[std::forward<std::string>(eventName)].emplace_back(std::forward<std::function<T>>(function));
    } catch (EventManagerExceptions exceptionCode) {
        if (exceptionCode == EventManagerExceptions::UnsusportedEventType) {
            SDL_Log("Listener add request to event %s with paramenters not allows", eventName.c_str());
        }
    } catch (...) {
        SDL_Log("Unhandled exception on adding listener to %s", eventName.c_str());
    }
} 

template <typename T>
void InternalEventManager::removeListeners(std::string&& eventName, std::function<T>&& function) noexcept {
    try {
        this->getEventMap(std::function<T> {}).erase(std::forward<std::string>(eventName));
    } catch (EventManagerExceptions exceptionCode) {
        if (exceptionCode == EventManagerExceptions::UnsusportedEventType) {
            SDL_Log("Listener remove request to event %s with paramenters not allows", eventName.c_str());
        }
    } catch (...) {
        SDL_Log("Unhandled exception on removing listener to %s", eventName.c_str());
    }
}

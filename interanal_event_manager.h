#ifndef INTERNAL_EVENT_MANAGER_H
#define INTERNAL_EVENT_MANAGER_H

#include <unordered_map>
#include <memory>
#include <vector>
#include <string>
#include <functional>

template<typename T>
using EventMap = std::unordered_map<std::string, std::vector<std::function<T>>;

enum class EventManagerExceptions {
    UnsusportedEventType = 7
};

class InternalEventManager {
    private:
        EventMap<void(int)> intEvents;
        EventMap<void(float)> floatEvents;

        template<typename T>
        EventMap<T>& getEventMap(std::function<T>&& f) {
            throw EventManagerExceptions::UnsusportedEventType;
        };

        EventMap<void(int)>& getEventMap(std::function<void(int)> f) noexcept { return intEvents; }
        EventMap<void(float)>& getEventMap(std::function<void(float)> f) noexcept { return floatEvents; }

    public:        
        template<typename T>
        void addListener(std::string&& eventName, std::function<T>&& function) noexcept;

        template<typename T>
        void removeListeners(const std::string& eventName, std::function<T>&& function) noexcept;

        template<typename ... Args>
        void invoke(const std::string&, Args&&... args) noexcept;

};

#endif
#ifndef INTERNAL_EVENT_MANAGER_H
#define INTERNAL_EVENT_MANAGER_H

#include <unordered_map>
#include <memory>
#include <vector>
#include <string>
#include <functional>

template<typename T>
using InternalEventMap = std::unordered_map<std::string, std::vector<std::function<T>>>;

enum class EventManagerExceptions {
    UnsusportedEventType = 7
};

class InternalEventManager {
    private:
        InternalEventMap<void(int)> intEvents;
        InternalEventMap<void(float)> floatEvents;

        template<typename T>
        InternalEventMap<T>& getEventMap(std::function<T>&& f) {
            throw EventManagerExceptions::UnsusportedEventType;
        };

        InternalEventMap<void(int)>& getEventMap(std::function<void(int)> f) noexcept { return intEvents; }
        InternalEventMap<void(float)>& getEventMap(std::function<void(float)> f) noexcept { return floatEvents; }

    public:        
        template<typename T>
        void addListener(std::string&& eventName, std::function<T>&& function) noexcept;

        template<typename T>
        void removeListeners(const std::string& eventName, std::function<T>&& function) noexcept;

        template<typename ... Args>
        void invoke(const std::string&, Args&&... args) noexcept;

};

#endif
#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include "interanal_event_manager.h"
#include "updatable_object.h"
#include "vector3D.h"
#include <stdexcept>

class GameObject : InternalEventManager, UpdatableObject {
    protected:
        Vector3D _position;
        Vector3D _size;
    
    public:
        template<typename T>
        GameObject(T&& position, T&& size) : InternalEventManager(), UpdatableObject() {
            if (!std::is_same<T, Vector3D>()) throw std::invalid_argument("Position and Size of of GameObject must be Vector3D");
            _position = std::forward<T>(position);
            _size = std::forward<T>(size);
        }

        virtual void Update() = 0;

};

#endif
#include "updatable_object.h"
#include <stdlib.h>
#include <list>

std::list<UpdatableObject*> UpdatableObject::UpdatableObjects = std::list<UpdatableObject*>();

UpdatableObject::UpdatableObject() {
    if (UpdatableObjects.empty())
        objectId = 0;
    else
        objectId = UpdatableObjects.back()->objectId + 1;
    UpdatableObjects.push_back(this);
}

void UpdatableObject::cleanup() {
    for (auto it = UpdatableObjects.begin(); it != UpdatableObjects.end(); it ++)
        delete *it;
    UpdatableObjects.clear();
}

void UpdatableObject::UpdateAll() {
    for (auto it = UpdatableObjects.begin(); it != UpdatableObjects.end(); it ++)
        (*it)->Update();
}
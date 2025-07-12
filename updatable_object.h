#ifndef UPDATABLE_OBJECT
#define UPDATABLE_OBJECT

#include <stdlib.h>
#include <list>

class UpdatableObject
{
    public: // Static
        static std::list<UpdatableObject*> UpdatableObjects;
        static void cleanup();
        static void UpdateAll();
    
    public: // Virtual
        virtual void Update() = 0;

    public: // Object Specific
        size_t objectId;
        UpdatableObject();
};

#endif
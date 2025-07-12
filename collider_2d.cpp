#include "collider_2d.h"
#include <stdlib.h>
#include <vector>

#define MINIMUM_NUMBER_OF_ELEMENTS 2
#define MAX_REDUNDANT_RECURSION 3

std::vector<Collider2D*> Collider2D::allCollider2D = std::vector<Collider2D*>();

bool Collider2D::isColliding(Collider2D collider1, Collider2D collider2) {
    return false;
}

void Collider2D::CheckAllCollisions() {
    // Getters initial data
    // Calls Collider2D::CheckAllCollisions with arguments
}

void Collider2D::CheckAllCollisions(int begin, int end, int axis, int endCount, float average) {
    Collider2D* swapValue;
    float minValue, maxValue;

    // Checks for endCount || Checks for nunmber of elements 
    if (end - begin <= MINIMUM_NUMBER_OF_ELEMENTS || endCount > MAX_REDUNDANT_RECURSION) {
        for (int i = begin; i <= end; i++)
            for (int j = i + 1; j <= end; j++)
                if (Collider2D::allCollider2D[i]->isColliding(Collider2D::allCollider2D[j])) {
                    Collider2D::allCollider2D[i]->onCollision();
                    Collider2D::allCollider2D[j]->onCollision();
                }
        return;
    }


    // sorts for collision in first half
        // Sorts array changing positions - All bebofre i colide and after dont
        // Gets the minimum and maximum central point on each axis of the colling part
    // calls Collider2D::CheckAllCollisions

    int discarded_size = 0;
    //Creates space collider

    for (int i = begin; i + discarded_size <= end;) {

        // Chceck if item[i] does not collide with the space collider
        if (Collider2D::allCollider2D[i]->getExtremeValue(axis, true) > average) {
            swapValue = allCollider2D[i];
            Collider2D::allCollider2D[i] = Collider2D::allCollider2D[end-discarded_size];
            Collider2D::allCollider2D[end-discarded_size] = swapValue;
            discarded_size ++;
            continue;
        }

        i++;
    }

    minValue = Collider2D::allCollider2D[begin]->getCentralValue(axis);
    maxValue = Collider2D::allCollider2D[begin]->getCentralValue(axis);
    for (Collider2D* p = Collider2D::allCollider2D[begin]; p != Collider2D::allCollider2D[end - discarded_size]; p++)
    {
        // Chaecks for extreme average values
        if (p->getExtremeValue(axis, true) < minValue)
            minValue = p->getExtremeValue(axis, true);
        
        if (p->getExtremeValue(axis, false) > maxValue)
            maxValue = p->getExtremeValue(axis, false);
    }
    
    if (end - discarded_size == begin) {
        return Collider2D::CheckAllCollisions(begin, end - discarded_size, (axis + 1) % 2, endCount + 1, (maxValue - minValue) / float(2));
    }

    Collider2D::CheckAllCollisions(begin, end - discarded_size, (axis + 1) % 2, 0, (maxValue - minValue) / float(2));


    // sorts for collision in second half - only first array
        // Sorts array changing positions - All after i colide and before dont
        // Gets the minimum and maximum central point on each axis of the colling part

    for (int i = begin; i <= discarded_size;) {

        // Chceck if item[i] collides with the space collider
        if (Collider2D::allCollider2D[i]->getExtremeValue(axis, false) > average) {
            discarded_size ++;
            swapValue = allCollider2D[i];
            Collider2D::allCollider2D[i] = Collider2D::allCollider2D[end-discarded_size];
            Collider2D::allCollider2D[end-discarded_size] = swapValue;
            continue;
        }

        i++;
    }

    minValue = Collider2D::allCollider2D[discarded_size]->getCentralValue(axis);
    maxValue = Collider2D::allCollider2D[discarded_size]->getCentralValue(axis);
    for (Collider2D* p = Collider2D::allCollider2D[discarded_size]; p != Collider2D::allCollider2D[end]; p++)
    {
        // Chaecks for extreme average values
        if (p->getExtremeValue(axis, true) < minValue)
            minValue = p->getExtremeValue(axis, true);
        
        if (p->getExtremeValue(axis, false) > maxValue)
            maxValue = p->getExtremeValue(axis, false);
    }

    // calls Collider2D::CheckAllCollisions
    Collider2D::CheckAllCollisions(begin, end - discarded_size, (axis + 1) % 2, 0, (maxValue - minValue) / float(2));
}
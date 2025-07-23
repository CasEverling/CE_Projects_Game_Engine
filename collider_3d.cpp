#include "collider_3d.h"
#include <stdlib.h>
#include <vector>
#include "vector2D.h"
#include <utility>
#include "Renderer.h"
#include <algorithm>
#include <functional>
#include "vector3D.h"
#include "vector2D.h"

#define MINIMUM_NUMBER_OF_ELEMENTS 2
#define MAX_REDUNDANT_RECURSION 3


std::vector<std::shared_ptr<Collider3D>> Collider3D::allCollider3D = std::vector<std::shared_ptr<Collider3D>>();

/* Handles creating and modifying collider */

Collider3D::Collider3D() {
    position = Vector3D{};
    reference = Vector3D{};
}



std::shared_ptr<Collider3D> Collider3D::create() {
    std::shared_ptr<Collider3D> pointer = std::make_shared<Collider3D>();
    allCollider3D.emplace_back(pointer);
    return pointer;
}


int Collider3D::getNumberOfColliders() { return Collider3D::allCollider3D.size(); }
void Collider3D::cleanup() { Collider3D::allCollider3D.clear(); }

bool Collider3D::isColliding(std::shared_ptr<Collider3D> collider1, std::shared_ptr<Collider3D> collider2) {
    // Check overlap along x-axis
    Vector2D x1 = collider1->getExtremeValue(0); // x-axis min/max
    Vector2D x2 = collider2->getExtremeValue(0);
    bool xOverlap = x1[0] < x2[1] && x2[0] < x1[1];

    // Check overlap along y-axis
    Vector2D y1 = collider1->getExtremeValue(1); // y-axis min/max
    Vector2D y2 = collider2->getExtremeValue(1);
    bool yOverlap = y1[0] < y2[1] && y2[0] < y1[1];

    Vector2D z1 = collider1->getExtremeValue(2); // z-axis min/max
    Vector2D z2 = collider2->getExtremeValue(2);
    bool zOverlap = z1[0] < z2[1] && z2[0] < z1[1];

    return xOverlap && yOverlap && zOverlap;
}

/* Handles collision detection */

void Collider3D::CheckAllCollisions() {
    if (!allCollider3D.size()) return;
    Collider3D::CheckAllCollisions(false);
}

void Collider3D::CheckAllCollisions(bool isMultiThread) {
    // Analysis of the collider's distribution
    float brute_force_size;

    float mean_x = 0, mean_y = 0;
    float sd_x = 0, sd_y = 0;
    float min_x = allCollider3D[0]->getCentralValue(0);
    float max_x = allCollider3D[0]->getCentralValue(0);

    int size = Collider3D::allCollider3D.size();

    for ( auto collider : allCollider3D ) {
        mean_x += collider->getCentralValue(0);
        mean_y += collider->getCentralValue(1);
    }

    mean_x /= size; mean_y /= size;

    for ( auto collider : allCollider3D ) {
        sd_x += std::abs(mean_x - collider->getCentralValue(0));
        sd_y += std::abs(mean_y - collider->getCentralValue(1));
    }

    sd_x /= size; sd_y /= size;

    // Calculates euristhics for picking best sample size for dividing the sample
    brute_force_size = (0.113 * (float) size / (sd_x + sd_y));
    brute_force_size *= brute_force_size;

    if (brute_force_size < 50) brute_force_size = 50;

    // Sets the position vector
    std::vector<std::size_t> indexes;
    indexes.resize(size);

    for (int i = 0; i < size; i++)
        indexes[i] = i;

    for (auto collider : allCollider3D) {
        min_x = std::min(min_x, collider->getCentralValue(0));
        max_x = std::max(max_x, collider->getCentralValue(0));
    }

    // Call recursive collision detection
    //if (isMultiThread)
    //   CheckAllCollisionsMultiThread(indexes, 0, size, 2, mean_x, brute_force_size);
    //else


    CheckAllCollisions(indexes, 0, 0, size, 2, brute_force_size / 1);

}

void Collider3D::CheckAllCollisions(std::vector<std::size_t>& indexes, int axis, int begin, int size, int endCount, int max_size) {
    if (size <= max_size || endCount > 2) {
        if (size > 1)
            return BruteForceCheck(indexes, begin, size);
        return;
    }

    int   lowerGorupSize = 0, upperGroupSize = 0;
    float lowerGroupSum = 0 , upperGroupSum = 0;
    int   currLower = 0     , currUpper = 0;

    float max = allCollider3D[indexes[begin]]->getCentralValue(axis);
    float min = max;

    float center_reference;
    for (int i = begin + 1; i < begin + size; i++) {
        center_reference = allCollider3D[indexes[i]]->getCentralValue(axis);
        max = std::max(max, center_reference);
        min = std::min(min, center_reference);
    }
    float average = (max + min) / (float) 2;


    Vector2D reference_values;
    for (int i = begin; i < begin + size; i++) {
        reference_values = allCollider3D[indexes[i]]->getExtremeValue(axis);

        if (reference_values[0] <= average)
            lowerGorupSize ++;

        if (reference_values[1] > average)
            upperGroupSize++;
    }    

    if (size == upperGroupSize || size == lowerGorupSize)
        return CheckAllCollisions(indexes, (axis+1)%3, begin, size, endCount + 1, max_size);


    while (currLower < lowerGorupSize)
        if (allCollider3D[indexes[begin+currLower]]->getExtremeValue(axis)[0] <= average)
            currLower ++;
        else {
            if (begin+currLower != begin + size -currUpper -1) {
                indexes[begin+currLower] ^= indexes[begin + size -currUpper -1];
                indexes[begin + size -currUpper -1] ^= indexes[begin+currLower];
                indexes[begin+currLower] ^= indexes[begin + size -currUpper -1];
            }
            currUpper ++;
        }

    CheckAllCollisions(indexes, (axis+1)%3, begin, lowerGorupSize, 0, max_size);

    for (int i = begin; i < lowerGorupSize;)
        if (allCollider3D[indexes[begin+currLower]]->getExtremeValue(axis)[1] > average) {
            i ++;
        } else {
            if (i != begin + lowerGorupSize -1) {
                indexes[i] ^= indexes[begin + lowerGorupSize -1];
                indexes[begin + lowerGorupSize -1] ^= indexes[i];
                indexes[i] ^= indexes[begin + lowerGorupSize-1];
            }
            lowerGorupSize --;
        }

    CheckAllCollisions(indexes, (axis+1)%3, begin + size - upperGroupSize, upperGroupSize, 0, max_size);
}  

void Collider3D::BruteForceCheck(std::vector<std::size_t>& indexes, int begin, int size) {
    for(int i = 0; i < size; i++)
        for(int j = i + 1; j < size; j++) {

            if (Collider3D::isColliding(allCollider3D[indexes[begin + i]], allCollider3D[indexes[begin + j]])) {
                allCollider3D[i]->onCollision(allCollider3D[indexes[begin + j]]);
                allCollider3D[j]->onCollision(allCollider3D[indexes[begin + i]]);
                Renderer::lightness += 1;
            }
                
        }
}

void Collider3D::BruteForceCheck(const std::vector<std::size_t>& indexes, int size) {
    for(int i = 0; i < size; i++)
        for(int j = i + 1; j < size; j++)
            if (Collider3D::isColliding(allCollider3D[i], allCollider3D[j])) {
                SDL_Log("%d and %d are colliding");
                Renderer::lightness += 1;
            }
                
}


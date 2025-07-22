#include "collider_2d.h"
#include <stdlib.h>
#include <vector>
#include "vector2D.h"
#include <utility>
#include "Renderer.h"
#include <algorithm>

#define MINIMUM_NUMBER_OF_ELEMENTS 2
#define MAX_REDUNDANT_RECURSION 3


std::vector<std::shared_ptr<Collider2D>> Collider2D::allCollider2D = std::vector<std::shared_ptr<Collider2D>>();
/* Handles creating and modifying collider */

Collider2D::Collider2D() {
    position = Vector2D(0,0);
    reference = Vector2D(0,0);
    type = ColliderType::BoxCollider;
}

std::shared_ptr<Collider2D> Collider2D::create() {
    std::shared_ptr<Collider2D> pointer = std::make_shared<Collider2D>();
    allCollider2D.emplace_back(pointer);
    return pointer;
}


int Collider2D::getNumberOfColliders() { return Collider2D::allCollider2D.size(); }
void Collider2D::cleanup() { Collider2D::allCollider2D.clear(); }

bool Collider2D::isColliding(std::shared_ptr<Collider2D> collider1, std::shared_ptr<Collider2D> collider2) {
    // Check overlap along x-axis
    Vector2D x1 = collider1->getExtremeValue(0); // x-axis min/max
    Vector2D x2 = collider2->getExtremeValue(0);
    bool xOverlap = x1[0] < x2[1] && x2[0] < x1[1];

    // Check overlap along y-axis
    Vector2D y1 = collider1->getExtremeValue(1); // y-axis min/max
    Vector2D y2 = collider2->getExtremeValue(1);
    bool yOverlap = y1[0] < y2[1] && y2[0] < y1[1];

    return xOverlap && yOverlap;
}

/* Handles collision detection */

void Collider2D::CheckAllCollisions() {
    if (!allCollider2D.size()) return;
    Collider2D::CheckAllCollisions(false);
}

void Collider2D::CheckAllCollisions(bool isMultiThread) {
    // Analysis of the collider's distribution
    float brute_force_size;

    float mean_x = 0, mean_y = 0;
    float sd_x = 0, sd_y = 0;
    float min_x = allCollider2D[0]->getCentralValue(0);
    float max_x = allCollider2D[0]->getCentralValue(0);

    int size = Collider2D::allCollider2D.size();

    for ( auto collider : allCollider2D ) {
        mean_x += collider->getCentralValue(0);
        mean_y += collider->getCentralValue(1);
    }

    mean_x /= size; mean_y /= size;

    for ( auto collider : allCollider2D ) {
        sd_x += std::abs(mean_x - collider->getCentralValue(0));
        sd_y += std::abs(mean_y - collider->getCentralValue(1));
    }

    sd_x /= size; sd_y /= size;

    // Calculates euristhics for picking best sample size for dividing the sample
    brute_force_size = (0.113 * (float) size / (sd_x + sd_y));
    brute_force_size *= brute_force_size;

    if (brute_force_size < 3) brute_force_size = 3;

    // Sets the position vector
    std::vector<std::size_t> indexes;
    indexes.resize(size);

    for (int i = 0; i < size; i++)
        indexes[i] = i;

    for (auto collider : allCollider2D) {
        min_x = std::min(min_x, collider->getCentralValue(0));
        max_x = std::max(max_x, collider->getCentralValue(0));
    }

    // Call recursive collision detection
    //if (isMultiThread)
    //   CheckAllCollisionsMultiThread(indexes, 0, size, 2, mean_x, brute_force_size);
    //else


    CheckAllCollisions(indexes, 0, 0, size, 2, (min_x + max_x) / 2, brute_force_size / 1);

}
/*
void Collider2D::CheckAllCollisionsMultiThread(const std::vector<std::size_t>& indexes, int axis, int size, int endCount, float average, int max_size) {
    if (size <= max_size || endCount > 2)
        return BruteForceCheck(indexes, size);

    std::vector<std::size_t> lowerGroup;
    lowerGroup.resize(size);
    int lowerGorupSize = 0;
    float lowerGroupSum = 0;
    std::future<void> lowerThread;

    std::vector<std::size_t> upperGroup;
    upperGroup.resize(size);
    int upperGroupSize = 0;
    float upperGroupSum = 0;
    std::future<void> upperThread;

    std::shared_ptr<Collider2D> currentCollider;
    Vector2D reference_values = Vector2D(0,0);
    float center_reference;

    for (const int index : indexes) {
        currentCollider = Collider2D::allCollider2D[index];
        reference_values = currentCollider->getExtremeValue(axis);
        center_reference = currentCollider->getCentralValue((axis+1)%2);
        
        if (reference_values[0] < average) {
            lowerGroup[lowerGorupSize++] = index;
            lowerGroupSum += center_reference;
        }

        if (reference_values[1] > average) {
            upperGroup[upperGroupSize++] = index;
            upperGroupSum += center_reference;
        }
    }

    if (lowerGorupSize >= size -1 || upperGroupSize >= size -1)
        endCount ++;
    else
        endCount = 0;


    if (lowerGorupSize > 1)
        lowerThread = std::async(
            std::launch::async,
            CheckAllCollisionsMultiThread, lowerGroup, (axis+1)%2, lowerGorupSize, endCount, (((float)lowerGroupSum) / lowerGorupSize), max_size
        );
    
    if (upperGroupSize > 1)
        upperThread = std::async(
            std::launch::async,
            CheckAllCollisionsMultiThread, upperGroup, (axis+1)%2, upperGroupSize, endCount, (((float)upperGroupSum) / upperGroupSize), max_size
        );
    
    lowerThread.wait();
    upperThread.wait();
}  
*/
void Collider2D::CheckAllCollisions(std::vector<std::size_t>& indexes, int axis, int begin, int size, int endCount, float average, int max_size) {
    SDL_Log("Average %f, size %d, endcount %d", average, size, endCount);
    if (size <= max_size || endCount > 2) {
        if (size > 1)
            return BruteForceCheck(indexes, begin, size);
        return;
    }

    int   lowerGorupSize = 0, upperGroupSize = 0;
    float lowerGroupSum = 0 , upperGroupSum = 0;
    int   currLower = 0     , currUpper = 0;

    float max = allCollider2D[indexes[begin]]->getCentralValue(axis);
    float min = max;

    float center_reference;
    for (int i = begin + 1; i < begin + size; i++) {
        center_reference = allCollider2D[indexes[i]]->getCentralValue(axis);
        max = std::max(max, center_reference);
        min = std::min(min, center_reference);
    }
    average = (max + min) / (float) 2;


    Vector2D reference_values = Vector2D(0,0);
    for (int i = begin; i < begin + size; i++) {
        reference_values = allCollider2D[indexes[i]]->getExtremeValue(axis);

        if (reference_values[0] <= average)
            lowerGorupSize ++;

        if (reference_values[1] > average)
            upperGroupSize++;
    }
    SDL_Log("Group Sizes: %d - %d - %d", lowerGorupSize, upperGroupSize, size);
    

    if (size == upperGroupSize || size == lowerGorupSize)
        return CheckAllCollisions(indexes, (axis+1)%2, begin, size, endCount + 1, 0, max_size);


    while (currLower < lowerGorupSize)
        if (allCollider2D[indexes[begin+currLower]]->getExtremeValue(axis)[0] <= average)
            currLower ++;
        else {
            if (begin+currLower != begin + size -currUpper -1) {
                indexes[begin+currLower] ^= indexes[begin + size -currUpper -1];
                indexes[begin + size -currUpper -1] ^= indexes[begin+currLower];
                indexes[begin+currLower] ^= indexes[begin + size -currUpper -1];
            }
            currUpper ++;
        }

    CheckAllCollisions(indexes, (axis+1)%2, begin, lowerGorupSize, 0, 0, max_size);

    for (int i = begin; i < lowerGorupSize;)
        if (allCollider2D[indexes[begin+currLower]]->getExtremeValue(axis)[1] > average) {
            i ++;
        } else {
            if (i != begin + lowerGorupSize -1) {
                indexes[i] ^= indexes[begin + lowerGorupSize -1];
                indexes[begin + lowerGorupSize -1] ^= indexes[i];
                indexes[i] ^= indexes[begin + lowerGorupSize-1];
            }
            lowerGorupSize --;
        }

    CheckAllCollisions(indexes, (axis+1)%2, begin + size - upperGroupSize, upperGroupSize, 0, 0, max_size);
}  

void Collider2D::BruteForceCheck(std::vector<std::size_t>& indexes, int begin, int size) {
    if (size < 2) return;
    SDL_Log("Beginin Brute Force, %d, size %d", begin, size);
    for(int i = 0; i < size; i++)
        for(int j = i + 1; j < size; j++) {

            if (Collider2D::isColliding(allCollider2D[indexes[begin + i]], allCollider2D[indexes[begin + j]])) {
                SDL_Log("%d and %d collider", indexes[begin + i],  indexes[begin + j]);
                Renderer::lightness += 1;
            } else {
                SDL_Log("%d and %d do not collide", indexes[begin + i], indexes[begin + j]);
            }
                
        }
}

void Collider2D::BruteForceCheck(const std::vector<std::size_t>& indexes, int size) {
    for(int i = 0; i < size; i++)
        for(int j = i + 1; j < size; j++)
            if (Collider2D::isColliding(allCollider2D[i], allCollider2D[j]))
                Renderer::lightness += 1;
                
}


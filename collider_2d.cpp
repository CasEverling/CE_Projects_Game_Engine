#include "collider_2d.h"
#include <stdlib.h>
#include <vector>
#include "vector2D.h"
#include <future>
#include <utility>

#define MINIMUM_NUMBER_OF_ELEMENTS 2
#define MAX_REDUNDANT_RECURSION 3

std::vector<std::shared_ptr<Collider2D>> Collider2D::allCollider2D = std::vector<std::shared_ptr<Collider2D>>();

/* Handles creating and modifying collider */




/* Handles collision detection */

void Collider2D::CheckAllCollisions(bool isMultiThread) {
    // Analysis of the collider's distribution
    int brute_force_size;

    float mean_x = 0, mean_y = 0;
    float sd_x = 0, sd_y = 0;

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
    brute_force_size = (0.113 * size / (sd_x + sd_y));
    brute_force_size *= brute_force_size;

    // Sets the position vector
    std::vector<std::size_t> indexes;
    indexes.resize(size);

    for (int i = 0; i < size; i++)
        indexes[i] = i;

    // Call recursive collision detection
    if (isMultiThread)
        CheckAllCollisionsMultiThread(indexes, 0, size, 2, mean_x, brute_force_size);
    else
        CheckAllCollisions(indexes, 0, 0, size, 2, mean_x, brute_force_size);

}

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

void Collider2D::CheckAllCollisions(std::vector<std::size_t>& indexes, int axis, int begin, int size, int endCount, float average, int max_size) {
    if (size <= max_size || endCount > 2)
        return BruteForceCheck(indexes, size);

    int lowerGorupSize = 0;
    float lowerGroupSum = 0;

    int upperGroupSize = 0;
    float upperGroupSum = 0;

    std::shared_ptr<Collider2D> currentCollider;
    Vector2D reference_values = Vector2D(0,0);
    float center_reference;

    while (lowerGorupSize < size - upperGroupSize) {
        currentCollider = Collider2D::allCollider2D[indexes[lowerGorupSize]];
        reference_values = currentCollider->getExtremeValue(axis);
        center_reference = currentCollider->getCentralValue((axis+1)%2);
        
        if (reference_values[0] < average) {
            lowerGorupSize ++;
            lowerGroupSum += center_reference;
        }

        if (reference_values[1] > average) {
            indexes[begin + lowerGorupSize] ^= indexes[begin + size - 1 - upperGroupSize];
            upperGroupSize ++;
        }
    }

    if (lowerGorupSize >= size -1 || lowerGorupSize <= 1)
        endCount ++;
    else
        endCount = 0;

    if (lowerGorupSize > 1)
        CheckAllCollisions( indexes, (axis+1)%2, begin, lowerGorupSize, endCount, (((float)lowerGroupSum) / lowerGorupSize), max_size );
    
    for( int i = begin; i < begin + lowerGorupSize; ) {
        currentCollider = Collider2D::allCollider2D[indexes[i]];
        reference_values = currentCollider->getExtremeValue(axis);
        
        if (reference_values[1] > average) {
            indexes[begin + lowerGorupSize] ^= indexes[begin + size - 1 - upperGroupSize];
            i ++;
        }

        if (reference_values[1] > average) {
            lowerGorupSize --;
            upperGroupSize ++;
        }
    }

    for (int i = begin + lowerGorupSize; i < begin + size; i++)
        upperGroupSum += Collider2D::allCollider2D[indexes[i]]->getCentralValue((axis+1)%2);

    if (upperGroupSize > 1)
        CheckAllCollisions( indexes, (axis+1)%2, begin + lowerGorupSize, upperGroupSize, endCount, (((float)upperGroupSum) / upperGroupSize), max_size );

}  

void Collider2D::BruteForceCheck(std::vector<std::size_t>& indexes, int begin, int size) {
    for(int i = 0; i < size; i++)
        for(int j = i + 1; j < size; j++)
            Collider2D::isColliding(allCollider2D[begin + i], allCollider2D[begin + j]);
}

void Collider2D::BruteForceCheck(const std::vector<std::size_t>& indexes, int size) {
    for(int i = 0; i < size; i++)
        for(int j = i + 1; j < size; j++)
            Collider2D::isColliding(allCollider2D[i], allCollider2D[j]);
}
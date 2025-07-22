#ifndef COLLIDER_3D_H
#define COLLIDER_3D_H

#include <stdlib.h>
#include <vector>
#include "vector2D.h"
#include "vector3D.h"

class Collider3D
{      
    public:
        Collider3D();

    public:        
        static std::shared_ptr<Collider3D> create();

        void onCollision();
        float getCentralValue(int axis) {
            return position[axis] + reference[axis] / 2;
            
        }

        Vector2D getExtremeValue(int axis) {
            return Vector2D(position[axis], position[axis] + reference[axis]);
        }
        
        Vector3D position;
        Vector3D reference;



    public:
        static bool isColliding(std::shared_ptr<Collider3D> collider1, std::shared_ptr<Collider3D> collider2);

        static void CheckAllCollisions();
        static void CheckAllCollisionsMultiThread();

        static void cleanup();
        static int  getNumberOfColliders();
    
    private:
        static void CheckAllCollisions(bool isMultiThread);

        static void CheckAllCollisions(std::vector<std::size_t>& indexes, int axis, int begin, int size, int endCount, int max_size);
        static void CheckAllCollisionsMultiThread(const std::vector<std::size_t>& indexes, int axis, int size, int endCount, int max_size);
        static void BruteForceCheck(const std::vector<std::size_t>& indexes, int size);
        static void BruteForceCheck(std::vector<std::size_t>& indexes, int begin, int size);

        static std::vector<std::shared_ptr<Collider3D>> allCollider3D;
};

#endif
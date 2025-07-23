#ifndef COLLIDER_3D_H
#define COLLIDER_3D_H

#include <stdlib.h>
#include <vector>
#include "vector2D.h"
#include "vector3D.h"
#include <functional>
#include <memory>
#include "Renderer.h"
#include <SDL3/SDL.h>

enum class Collider3DType {
    RigidBodyColldier,
    FieldCollider
};

class Collider3D
{      
    public:
        Collider3D();

        void remove() {
            for (int i = 0; i < allCollider3D.size(); i++) {
                if (allCollider3D[i].get() == this) {
                    std::swap(allCollider3D[i], allCollider3D.back());
                    allCollider3D.pop_back();
                    break;
                }
            }
            throw "Collider should not exist only outside of allColliders3D";
        }

    private:
        std::vector<std::function<void(const std::shared_ptr<Collider3D>)>> callbacks = std::vector<std::function<void(const std::shared_ptr<Collider3D>)>>();

    public:        
        static std::shared_ptr<Collider3D> create();

        float getCentralValue(int axis) {
            return position[axis] + reference[axis] / 2;
            
        }

        Vector2D getExtremeValue(int axis) {
            return Vector2D(position[axis], position[axis] + reference[axis]);
        }
        
        Vector3D position;
        Vector3D reference;
        
        void addCallback(std::function<void(const std::shared_ptr<Collider3D>)> callback) {
            SDL_Log("Calback function add to collider");
            callbacks.emplace_back(callback); 
        }

        void onCollision(std::shared_ptr<Collider3D> collider) {
            for (auto callback : callbacks) {
                Renderer::lightness ++;
                callback(collider);
            }
        }
    
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
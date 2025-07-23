#ifndef RIGID_BODY_3D
#define RIGID_BODY_3D

#include "vector3D.h"
#include "game_object.h"
#include "collider_3d.h"
#include <cmath>

class RigidBody3D {
    private:
        std::shared_ptr<GameObject> parent;
        Vector3D velocity;
        Vector3D acceleration;
        float mass;
        
        void OnCollision(std::weak_ptr<Collider3D> OtherColldier, Vector3D offset) {
            // calculates how to procede based on that 
            // Note: keep reference to own collider
        };

        /* Acessible by parent's internal event manager */
        void SetVelocity(Vector3D&& newVelocity) {
            velocity = std::forward<Vector3D>(newVelocity);
        }

        void SetAcceleration(Vector3D&& newAcceleration) {
            acceleration = std::forward<Vector3D>(newAcceleration);
        }

        void ApplyForce(Vector3D force) {
            acceleration += force / mass;
        }

        void AddCollider(std::shared_ptr<Collider3D> colldier);

    public:
        void Update();

        float getMomentum() const { 
            return velocity.get_magnitude() * mass; 
        }

        Vector3D getVelocity() const {
            return velocity;
        }

        Vector3D getAcceleration() const {
            return acceleration;
        }

        float getKneticEnergy() const {
            0.5 * std::pow(getMomentum(), 2) / mass;
        }
};

#endif
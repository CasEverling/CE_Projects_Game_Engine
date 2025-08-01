#ifndef RIGID_BODY_2D
#define RIGID_BODY_2D

#include "vector2D.h"
#include "game_object.h"
#include "collider_2d.h"
#include <cmath>

class RigidBody2D {
    private:
        std::shared_ptr<GameObject> parent;
        Vector2D velocity;
        Vector2D acceleration;
        float mass;
        
        void OnCollision(std::weak_ptr<Collider2D> OtherColldier, Vector2D offset) {
            // calculates how to procede based on that 
            // Note: keep reference to own collider
        };

        /* Acessible by parent's internal event manager */
        void SetVelocity(Vector2D&& newVelocity) {
            velocity = std::forward<Vector2D>(newVelocity);
        }

        void SetAcceleration(Vector2D&& newAcceleration) {
            acceleration = std::forward<Vector2D>(newAcceleration);
        }

        void ApplyForce(Vector2D force) {
            acceleration += force / mass;
        }

        void AddCollider(std::shared_ptr<Collider2D> colldier);

    public:
        void Update();

        float getMomentum() const { 
            return velocity.get_magnitude() * mass; 
        }

        Vector2D getVelocity() const {
            return velocity;
        }

        Vector2D getAcceleration() const {
            return acceleration;
        }

        float getKneticEnergy() const {
            0.5 * std::pow(getMomentum(), 2) / mass;
        }
};

#endif
#include "vector3D.h"
#include "rigidbody3D.h"
#include <stdexcept>
#include <algorithm>

class movementDirectionManaget {
    private:
        std::shared_ptr<RigidBody3D> rigidBody3D;
        Vector3D _targetDirection {};
        float acceleration;
    public:
        template<typename T>
        void updateDirection(T&& targetDirection) {
            if (!std::is_same<T, Vector3D>()) throw std::invalid_argument("Parameter must be od Type Vector3D");
            targetDirection = std::forward<Vector3D>(targetDirection);
        }

        movementDirectionManaget(std::shared_ptr<RigidBody3D> rb) : rigidBody3D(rb) {}

        void Update() {
            Vector3D difference = _targetDirection - rigidBody3D->getVelocity();
            rigidBody3D->SetAcceleration(
                std::min(difference.unitVector() * acceleration, difference);
            );
        }
}
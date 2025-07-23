#include <stdlib.h>
#include <SDL3/SDL.h>
#include "character.h"
#include "resource_manager.h"
#include "Renderer.h"
#include "time.h"
#include "sprite_renderer.h"
#include "event_manager.h"
#include "collider_2d.h"
#include "collider_3d.h"
#include <algorithm>

Character::Character(float velocity_x, float velocity_y, float position_x, float position_y) 
  : GameObject(Vector3D(position_x, position_y, 0), Vector3D(1,1,1)),
    spriteRenderer("block_x_3") 
{

    this->velocity_x = velocity_x;
    this->velocity_y = velocity_y;
    this->position_x = position_x;
    this->position_y = position_y;

    spriteRenderer.layer = InGame_Layer;
    spriteRenderer.setTextureName("block_x_3");
    EventManager::AddListener("move_right", [this](){move_right();});
    EventManager::AddListener("move_left", [this](){move_left();});
    EventManager::AddListener("move_up", [this](){move_up();});
    EventManager::AddListener("move_down", [this](){move_down();});

    collider = Collider3D::create();
    collider->position = _position;
    collider->reference = _size;

    collider->addCallback([this](std::shared_ptr<Collider3D> otherCollider) {
        Vector3D overlap = Vector3D(
            std::min(otherCollider->getExtremeValue(0)[1], this->collider->getExtremeValue(0)[1]) - std::max(otherCollider->getExtremeValue(0)[0], this->collider->getExtremeValue(0)[0]),
            std::min(otherCollider->getExtremeValue(1)[1], this->collider->getExtremeValue(1)[1]) - std::max(otherCollider->getExtremeValue(1)[0], this->collider->getExtremeValue(1)[0]),
            std::min(otherCollider->getExtremeValue(2)[1], this->collider->getExtremeValue(2)[1]) - std::max(otherCollider->getExtremeValue(2)[0], this->collider->getExtremeValue(2)[0])
        );

        if (!overlap[0] || !overlap[1] || !overlap[2]) return;

        // Find minimum axis
        float ox = std::abs(overlap[0]);
        float oy = std::abs(overlap[1]);
        float oz = std::abs(overlap[2]);

        if (overlap.get_x() < overlap.get_y() && overlap.get_x() < overlap.get_z()) {
            overlap.set_y(0);
            overlap.set_z(0);
        } else if (oy < oz) {
            overlap.set_x(0);
            overlap.set_z(0);
        } else {
            overlap.set_x(0);
            overlap.set_y(0);
        }

        overlap = Vector3D(
            (otherCollider->getCentralValue(0) < this->collider->getCentralValue(0) ? overlap[0] : -overlap[0]),
            (otherCollider->getCentralValue(1) < this->collider->getCentralValue(1) ? overlap[1] : -overlap[1]),
            (otherCollider->getCentralValue(2) < this->collider->getCentralValue(2) ? overlap[2] : -overlap[2])
        );

        this->_position += overlap;
        this->collider->position = this->_position;
    });

}

Character::~Character() {
    //EventManager::RemoveListener("move_right", [this](){this->print();}, callback_value);
}

void Character::Update() {
    float tempVelocityx = velocity_x;
    float tempVelocityY = velocity_y;

    if (freeze) {velocity_x = 0; velocity_y = 0;}
    _position.set_x(_position.get_x() + velocity_x * Time::deltatime);
    _position.set_y(_position.get_y() + velocity_y * Time::deltatime);

    freeze = false;
    collider->position = _position;

    velocity_x = tempVelocityx;
    velocity_y = tempVelocityY;

    spriteRenderer.x = _position.get_x();
    spriteRenderer.y = _position.get_y();
    spriteRenderer.z = _position.get_z();
}


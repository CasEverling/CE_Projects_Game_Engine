#include <stdlib.h>
#include <SDL3/SDL.h>
#include "character.h"
#include "resource_manager.h"
#include "Renderer.h"
#include "time.h"
#include "sprite_renderer.h"
#include "event_manager.h"

Character::Character(float velocity_x, float velocity_y, float position_x, float position_y) 
  : UpdatableObject(),
    spriteRenderer("mage_x_3") 
{

    this->velocity_x = velocity_x;
    this->velocity_y = velocity_y;
    this->position_x = position_x;
    this->position_y = position_y;

    spriteRenderer.layer = InGame_Layer;
    spriteRenderer.setTextureName("mage_x_3");
    EventManager::AddListener("move_right", [this](){move_right();});
    EventManager::AddListener("move_left", [this](){move_left();});
    EventManager::AddListener("move_up", [this](){move_up();});
    EventManager::AddListener("move_down", [this](){move_down();});
}

Character::~Character() {
    //EventManager::RemoveListener("move_right", [this](){this->print();}, callback_value);
}

void Character::Update() {
    position_x += velocity_x * Time::deltatime;
    position_y += velocity_y * Time::deltatime;

    spriteRenderer.x = position_x;
    spriteRenderer.y = position_y;
}


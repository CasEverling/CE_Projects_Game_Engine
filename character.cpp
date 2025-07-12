#include <stdlib.h>
#include <SDL3/SDL.h>
#include "character.h"
#include "resource_manager.h"
#include "Renderer.h"
#include "time.h"
#include "sprite_renderer.h"
#include "event_manager.h"

#define SIN 6
#define COS 12

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480


Character::Character(float velocity_x, float velocity_y, float position_x, float position_y) 
  : UpdatableObject(),
    spriteRenderer("astronaut") 
{

    this->velocity_x = velocity_x;
    this->velocity_y = velocity_y;
    this->position_x = position_x;
    this->position_y = position_y;

    spriteRenderer.layer = InGame_Layer;
    spriteRenderer.setTextureName("astronaut");
    EventManager::AddListener("move_right", [this](){this->velocity_x += 10;});
    EventManager::AddListener("move_left", [this](){this->velocity_x -= 10;});
    EventManager::AddListener("move_up", [this](){this->velocity_y += 10;});
    EventManager::AddListener("move_down", [this](){this->velocity_y -= 10;});
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


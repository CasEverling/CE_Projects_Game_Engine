#ifndef BLOCK_H
#define BLOCK_H

#include "updatable_object.h"
#include "sprite_renderer.h"
#include "collider_2d.h"
#include "collider_3d.h"
#include "game_object.h"
#include "vector3D.h"

#include <SDL3/SDL.h>


class Block : public GameObject {
    private:
        SpriteRenderer spriteRenderer;
        std::shared_ptr<Collider3D> collider;

    public:
        Block(float position_x, float position_y, float position_z) :
            GameObject(Vector3D(position_x, position_y, position_z), Vector3D(1,1,1)), spriteRenderer("block") {
                spriteRenderer.setTextureName("block_x_3");
                spriteRenderer.layer = InGame_Layer;
                spriteRenderer.x = _position.get_x();
                spriteRenderer.y = _position.get_y();
                spriteRenderer.z = _position.get_z();

                collider = Collider3D::create();
                collider->position = _position;
                collider->reference = _size;
                SDL_Log("Collider at %f, %f, size 1, 1", position_x, position_y);
            };
    
        ~Block();
        void Update() override {};
};

#endif
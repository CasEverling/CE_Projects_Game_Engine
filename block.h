#ifndef BLOCK_H
#define BLOCK_H

#include "updatable_object.h"
#include "sprite_renderer.h"
#include "collider_2d.h"
#include "collider_3d.h"

#include <SDL3/SDL.h>


class Block : public UpdatableObject {
    private:
        float x, y, z;
        SpriteRenderer spriteRenderer;
        std::shared_ptr<Collider3D> collider;

    public:
        Block(float position_x, float position_y, float position_z) :
            x(position_x), y(position_y), z(position_z), spriteRenderer("block") {
                spriteRenderer.setTextureName("block_x_3");
                spriteRenderer.layer = InGame_Layer;
                spriteRenderer.x = x;
                spriteRenderer.y = y;
                spriteRenderer.z = z;

                collider = Collider3D::create();
                collider->position = Vector3D(x, y, z);
                collider->reference = Vector3D(1,1,1);
                SDL_Log("Collider at %f, %f, size 1, 1", position_x, position_y);
            };
    
        ~Block();
        void Update() override {};
};

#endif
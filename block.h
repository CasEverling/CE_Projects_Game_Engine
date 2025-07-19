#ifndef BLOCK_H
#define BLOCK_H

#include "updatable_object.h"
#include "sprite_renderer.h"

#define SIN 6
#define COS 11
#define HEIGHT 15

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

class Block : public UpdatableObject {
    private:
        float x, y, z;
        SpriteRenderer spriteRenderer;
    public:
        Block(float position_x, float position_y, float position_z) :
            x(position_x), y(position_y), z(position_z), spriteRenderer("block") {
                spriteRenderer.setTextureName("block_x_3");
                spriteRenderer.layer = InGame_Layer;
                spriteRenderer.x = x;
                spriteRenderer.y = y;
                spriteRenderer.z = z;
            };
    
        ~Block();
        void Update() override {};
};

#endif
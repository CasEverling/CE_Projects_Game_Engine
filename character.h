#include "updatable_object.h"
#include "sprite_renderer.h"
#include <stdlib.h>
#include <string>
#include <SDL3/SDL.h>
#include "event_manager.h"
#include "collider_2d.h"
#include "collider_3d.h"
#include "game_object.h"

class Character : public GameObject {
    private:
        bool freeze = false;
        std::string sprite;
        float velocity_x, velocity_y;
        float position_x, position_y;
        SpriteRenderer spriteRenderer;
        std::shared_ptr<Collider3D> collider;

    public:
        Character(float velocity_x, float velocity_y, float position_x, float position_y);
        ~Character();
        void Update() override;

        void move_right() { velocity_x += 10; }
        void move_left () { velocity_x -= 10; }
        void move_up   () { velocity_y -= 10; }
        void move_down () { velocity_y += 10; }
};
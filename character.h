#include "updatable_object.h"
#include "sprite_renderer.h"
#include <stdlib.h>
#include <string>
#include <SDL3/SDL.h>
#include "event_manager.h"

class Character : public UpdatableObject {
    private:
        std::string sprite;
        float velocity_x, velocity_y;
        float position_x, position_y;
        SpriteRenderer spriteRenderer;
        cbId callback_value_1, callback_value_2;
        void change_x();
        void change_y();

    public:
        Character(float velocity_x, float velocity_y, float position_x, float position_y);
        ~Character();
        void Update() override;
};
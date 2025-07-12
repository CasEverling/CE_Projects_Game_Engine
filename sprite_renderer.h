#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <stdlib.h>
#include <vector>
#include <string>

enum RenderingLayer {
    UI_Layer,
    InGame_Layer
};

class SpriteRenderer {
    /* STATIC */
    private:
        static std::vector<SpriteRenderer*> spriteRenderers;
        static void quickSortSpriteRenderers(int begin, int end);

    public:
        static void displayAll();
        static void cleanUp();

    /* OBJECT DEPENDENT */
    private:
        std::string textureName;
        float texture_width, texture_height;

    public:        
        RenderingLayer layer;
        float x, y, z;

        SpriteRenderer(std::string texture);
        ~SpriteRenderer();

        void display();

        bool isGreaterThan(SpriteRenderer* other); 

        void setTextureName(std::string texture_name);


};

#endif
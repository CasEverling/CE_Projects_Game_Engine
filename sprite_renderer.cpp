#include "sprite_renderer.h"
#include "resource_manager.h"
#include "Renderer.h"
#include <stdlib.h>
#include <vector>
#include <random>
#include <chrono>
#include <string>
#include <SDL3/SDL.h>

/* DEFINES THE PERSPECTIVE OF THE GAME */

#define SIN 7*3
#define COS 12*3
#define HEIGHT 15*3


/* Camera OffSet temporarely described here*/
#define WINDOW_WIDTH 1000/2 - 250
#define WINDOW_HEIGHT 800/2

/* STATIC */
std::vector<SpriteRenderer*> SpriteRenderer::spriteRenderers = std::vector<SpriteRenderer*>(); 

void SpriteRenderer::quickSortSpriteRenderers(int begin, int end) {
    int pivot, left_index, right_index;
    SpriteRenderer* swapVariable;

    // Checks if array is sorted
    if (end - begin == 1) return;

    for (int i = begin + 1; i < end; i++) {
        if (spriteRenderers[i-1]->isGreaterThan(spriteRenderers[i])) break;
        if (end - i == 1) return; 
    }

    // Pick random pivot point
    static std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> dist(begin, end-1);
    pivot = dist(rng);

    // Quick Sort
    swapVariable = spriteRenderers[pivot];
    spriteRenderers[pivot] = spriteRenderers[begin];
    spriteRenderers[begin] = swapVariable;

    left_index = begin + 1; right_index = end -1;

    while (left_index <= right_index) {
        if (spriteRenderers[left_index]->isGreaterThan(spriteRenderers[begin])) {
            swapVariable = spriteRenderers[left_index];
            spriteRenderers[left_index] = spriteRenderers[right_index];
            spriteRenderers[right_index] = swapVariable;
            right_index --;
        } else {
            left_index += 1;
        }
    }

    SpriteRenderer::quickSortSpriteRenderers(begin, left_index);
    SpriteRenderer::quickSortSpriteRenderers(right_index, end);
}

void SpriteRenderer::cleanUp() {
    SpriteRenderer::spriteRenderers.clear();
}

void SpriteRenderer::displayAll() {
    quickSortSpriteRenderers(0, SpriteRenderer::spriteRenderers.size());
    for (auto it = SpriteRenderer::spriteRenderers.begin(); it != SpriteRenderer::spriteRenderers.end(); it++)
        (*it)->display();
}

/* OBJECT DEPENDENT */
bool SpriteRenderer::isGreaterThan(SpriteRenderer* other) {
    if (layer != other->layer)
        return layer > other->layer;
    
    if (z != other->z)
        return z > other->z;
        
    return x + y > other->x + other->y;

}

void SpriteRenderer::display() {
    SDL_Texture* texture = ResourceManager::get_texture(
        textureName
    );
    SDL_GetTextureSize(texture, &texture_width, &texture_height);

    SDL_FRect dst_rect;
    switch (layer)
    {
    case InGame_Layer:
        dst_rect.x = (x - y) * COS + WINDOW_WIDTH;
        dst_rect.y = (x + y) * SIN - z * HEIGHT + WINDOW_HEIGHT - texture_height;
        dst_rect.w = texture_width;
        dst_rect.h = texture_height;
        break;
    
    case UI_Layer:
        dst_rect.x = x;
        dst_rect.y = y ;
        dst_rect.w = texture_width;
        dst_rect.h = texture_height;
        break;

    default:
        SDL_Log("Invalid layer %d", layer);
        break;
    }
    
    SDL_RenderTexture(Renderer::renderer, texture, NULL, &dst_rect);
}

SpriteRenderer::SpriteRenderer(std::string) {
    x = 0;
    y = 0;
    z = 0;

    SDL_Texture* texture = ResourceManager::get_texture(
        textureName
    );
    SDL_GetTextureSize(texture, &texture_width, &texture_height);
    spriteRenderers.emplace_back(this);
}

SpriteRenderer::~SpriteRenderer() {
    for (int i = 0; i < spriteRenderers.size(); i++)
        if (spriteRenderers[i] == this) {
            std::swap(
                spriteRenderers[spriteRenderers.size()-1],
                spriteRenderers[i]
            );
            spriteRenderers.pop_back();
        }

}

void SpriteRenderer::setTextureName(std::string texture_name) {
    textureName = texture_name;
}
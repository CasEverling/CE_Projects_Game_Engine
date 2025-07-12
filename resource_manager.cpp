#include "resource_manager.h"
#include <list>
#include <string>
#include <SDL3/SDL.h>
#include "Renderer.h"

std::list<DictionaryNode<SDL_Texture>> ResourceManager::textures = std::list<DictionaryNode<SDL_Texture>>();

void ResourceManager::cleanup()
{
    for (auto node = textures.begin(); node != textures.end(); node++)
        if (node->content)
            SDL_DestroyTexture(node->content);
    textures.clear();
}

SDL_Texture* ResourceManager::get_texture(const std::string& texture_name) {
    for (auto node = textures.begin(); node != textures.end(); node++)
        if (node->key == texture_name)
            return node->content;

    return load_texture(texture_name);
}

SDL_Texture* ResourceManager::load_texture(const std::string& texture_name) {
    SDL_Surface* surface = nullptr;
    SDL_Texture* texture = nullptr;
    char* bmp_path = nullptr;

    SDL_asprintf(&bmp_path, "%s%s.bmp", SDL_GetBasePath(), texture_name.c_str());
    surface = SDL_LoadBMP(bmp_path);
    SDL_free(bmp_path);

    if (!surface) {
        SDL_Log("Couldn't load bitmap: %s", SDL_GetError());
        return nullptr;
    }

    texture = SDL_CreateTextureFromSurface(Renderer::renderer, surface);
    SDL_DestroySurface(surface);

    if (!texture) {
        SDL_Log("Couldn't create texture: %s", SDL_GetError());
        return nullptr;
    }

    textures.push_back({texture_name, texture});
    return texture;
}

void ResourceManager::unload_texture(const std::string& texture_name) {
    for (auto node = textures.begin(); node != textures.end(); node++)
    {
        if (node->key == texture_name)
        {
            SDL_DestroyTexture(node->content);
            textures.erase(node);
            return;
        }
    }
    SDL_Log("Couldn't find texture: %s", texture_name.c_str());
}
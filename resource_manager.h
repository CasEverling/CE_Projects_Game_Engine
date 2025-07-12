#include <string>
#include <list>
#include <SDL3/SDL.h>

template<typename T>
struct DictionaryNode
{
    std::string key;
    T* content;
};

class ResourceManager
{
public:
    static SDL_Texture* get_texture(const std::string& texture_name);
    static void cleanup();
private:
    static std::list<DictionaryNode<SDL_Texture>> textures;
    static SDL_Texture* load_texture(const std::string& texture_name);
    static void unload_texture(const std::string& texture_name);
};

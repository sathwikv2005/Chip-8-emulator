#pragma
#include <SDL2/SDL.h>

#include "chip8.hpp"

class Renderer {
   public:
    Renderer(Chip8& chip8);
    ~Renderer();

    void render();
    bool processInput();

   private:
    Chip8& cpu;

    SDL_Window* window{};
    SDL_Renderer* renderer{};
};
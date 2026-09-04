#pragma
#include "chip8.hpp"
class Renderer {
   public:
    Renderer(Chip8& chip8);
    void render();

   private:
    Chip8& cpu;
};
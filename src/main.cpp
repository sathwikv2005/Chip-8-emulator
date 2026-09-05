#include <iostream>

#include "chip8.hpp"
#include "renderer.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: chip8 <rom>\n";
        return 1;
    }

    Chip8 chip8;
    Renderer renderer(chip8);

    chip8.loadROM(argv[1]);

    while (true) {
        chip8.cycle();
        renderer.render();
    }

    return 0;
}
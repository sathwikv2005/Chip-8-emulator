#include "renderer.hpp"

int main() {
    Chip8 chip8;
    Renderer renderer(chip8);

    chip8.loadROM("../roms/IBM Logo.ch8");

    // while (true) {
    //     chip8.cycle();
    //     renderer.render();
    // }
}
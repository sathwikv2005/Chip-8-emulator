#include <bits/chrono.h>

#include <iostream>

#include "chip8.hpp"
#include "renderer.hpp"

#define CPU_CYCLES_PER_SEC 500

void run(const char* path) {
    Chip8 chip8;
    Renderer renderer(chip8);

    chip8.loadROM(path);

    auto lastCycle = std::chrono::steady_clock::now();

    // cpu cycles per second
    const double cycleTime = 1.0 / CPU_CYCLES_PER_SEC;

    while (true) {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration<double>(now - lastCycle);
        if (elapsed.count() >= cycleTime) {
            chip8.cycle();

            // add cycleTime instead of setting it to `now` to prevent timing
            // drifts.
            lastCycle +=
                std::chrono::duration_cast<std::chrono::steady_clock::duration>(
                    std::chrono::duration<double>(cycleTime));
        }
        renderer.render();
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: chip8 <rom>\n";
        return 1;
    }

    run(argv[1]);

    return 0;
}
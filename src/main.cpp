#include <bits/chrono.h>

#include <iostream>

#include "chip8.hpp"
#include "renderer.hpp"

#define CPU_CYCLES_PER_SEC 500
#define DISPLAY_REFRESH_RATE 60

void run(const char* path) {
    Chip8 chip8;
    Renderer renderer(chip8);

    chip8.loadROM(path);

    auto lastCycle = std::chrono::steady_clock::now();
    auto lastRender = std::chrono::steady_clock::now();

    const double cyclePeriod = 1.0 / CPU_CYCLES_PER_SEC;

    const double renderPeriod = 1.0 / DISPLAY_REFRESH_RATE;

    while (true) {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration<double>(now - lastCycle);
        if (elapsed.count() >= cyclePeriod) {
            chip8.cycle();

            // add cyclePeriod instead of setting it to `now` to prevent timing
            // drift.
            lastCycle +=
                std::chrono::duration_cast<std::chrono::steady_clock::duration>(
                    std::chrono::duration<double>(cyclePeriod));
        }

        auto renderElapsed = std::chrono::duration<double>(now - lastRender);
        if (renderElapsed.count() >= renderPeriod) {
            renderer.render();

            // add renderPeriod instead of setting it to `now` to prevent timing
            // drift.
            lastRender +=
                std::chrono::duration_cast<std::chrono::steady_clock::duration>(
                    std::chrono::duration<double>(renderPeriod));
        }
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
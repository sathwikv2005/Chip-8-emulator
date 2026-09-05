#include <chrono>
#include <iostream>

#include "chip8.hpp"
#include "renderer.hpp"

#define CPU_CYCLES_PER_SEC 500
#define DISPLAY_REFRESH_RATE 60
#define TIMER_DECAY 60

#define TO_STEADY_DURATION(period)                                    \
    (std::chrono::duration_cast<std::chrono::steady_clock::duration>( \
        std::chrono::duration<double>(period)))

void run(const char* path) {
    Chip8 chip8;
    Renderer renderer(chip8);

    chip8.loadROM(path);

    auto lastCycle = std::chrono::steady_clock::now();
    auto lastRender = std::chrono::steady_clock::now();
    auto lastDelayDecay = std::chrono::steady_clock::now();
    auto lastSoundDecay = std::chrono::steady_clock::now();

    const double cyclePeriod = 1.0 / CPU_CYCLES_PER_SEC;
    const double renderPeriod = 1.0 / DISPLAY_REFRESH_RATE;
    const double timerPeriod = 1.0 / TIMER_DECAY;

    while (true) {
        renderer.processInput();

        auto now = std::chrono::steady_clock::now();

        while (std::chrono::duration<double>(now - lastCycle).count() >=
               cyclePeriod) {
            chip8.cycle();

            // add cyclePeriod instead of setting it to `now` to prevent timing
            // drift.
            lastCycle += TO_STEADY_DURATION(cyclePeriod);
        }

        while (std::chrono::duration<double>(now - lastRender).count() >=
               renderPeriod) {
            renderer.render();

            // add renderPeriod instead of setting it to `now` to prevent timing
            // drift.
            lastRender += TO_STEADY_DURATION(renderPeriod);
        }

        while (std::chrono::duration<double>(now - lastDelayDecay).count() >=
               timerPeriod) {
            chip8.decayDelayTimer();

            // add timerPeriod instead of setting it to `now` to prevent timing
            // drift.
            lastDelayDecay += TO_STEADY_DURATION(timerPeriod);
        }

        while (std::chrono::duration<double>(now - lastSoundDecay).count() >=
               timerPeriod) {
            chip8.decaySoundTimer();

            // add timerPeriod instead of setting it to `now` to prevent timing
            // drift.
            lastSoundDecay += TO_STEADY_DURATION(timerPeriod);
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
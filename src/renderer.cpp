#include "Renderer.hpp"

#include <stdexcept>

Renderer::Renderer(Chip8& chip8) : cpu(chip8) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) throw std::runtime_error(SDL_GetError());

    window =
        SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_CENTERED,
                         SDL_WINDOWPOS_CENTERED, 640, 320, SDL_WINDOW_SHOWN);

    if (!window) throw std::runtime_error(SDL_GetError());

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) throw std::runtime_error(SDL_GetError());
}

Renderer::~Renderer() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Renderer::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    const auto& display = cpu.getDisplay();

    for (int y = 0; y < 32; ++y) {
        for (int x = 0; x < 64; ++x) {
            uint64_t mask = uint64_t{1} << (63 - x);

            if (display[y] & mask) {
                SDL_Rect pixel{x * 10, y * 10, 10, 10};

                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                SDL_RenderFillRect(renderer, &pixel);
            }
        }
    }

    SDL_RenderPresent(renderer);
}
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

void Renderer::processInput() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type != SDL_KEYDOWN && event.type != SDL_KEYUP) continue;

        bool pressed = event.type == SDL_KEYDOWN;

        switch (event.key.keysym.sym) {
            case SDLK_1:
                cpu.setKey(0x1, pressed);
                break;
            case SDLK_2:
                cpu.setKey(0x2, pressed);
                break;
            case SDLK_3:
                cpu.setKey(0x3, pressed);
                break;
            case SDLK_4:
                cpu.setKey(0xC, pressed);
                break;

            case SDLK_q:
                cpu.setKey(0x4, pressed);
                break;
            case SDLK_w:
                cpu.setKey(0x5, pressed);
                break;
            case SDLK_e:
                cpu.setKey(0x6, pressed);
                break;
            case SDLK_r:
                cpu.setKey(0xD, pressed);
                break;

            case SDLK_a:
                cpu.setKey(0x7, pressed);
                break;
            case SDLK_s:
                cpu.setKey(0x8, pressed);
                break;
            case SDLK_d:
                cpu.setKey(0x9, pressed);
                break;
            case SDLK_f:
                cpu.setKey(0xE, pressed);
                break;

            case SDLK_z:
                cpu.setKey(0xA, pressed);
                break;
            case SDLK_x:
                cpu.setKey(0x0, pressed);
                break;
            case SDLK_c:
                cpu.setKey(0xB, pressed);
                break;
            case SDLK_v:
                cpu.setKey(0xF, pressed);
                break;
        }
    }
}
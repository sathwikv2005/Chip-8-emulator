#pragma once
#include <array>
#include <cstdint>
#include <string>

class Chip8 {
   public:
    Chip8();
    void loadROM(const std::string& path);

   private:
    // 4KB RAM
    std::array<uint8_t, 4096> memory{};
};
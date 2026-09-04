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

    // a chip-8 program starts from the memory address 0x200.
    uint16_t pc{0x200};

    // current opcode
    uint16_t opcode{};

    void executeOpCode();
};
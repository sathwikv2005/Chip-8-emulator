#pragma once
#include <array>
#include <cstdint>
#include <string>

class Chip8 {
   public:
    Chip8();
    void loadROM(const std::string& path);
    void cycle();

    const std::array<uint64_t, 32>& getDisplay() const;
    void clearDisplay();

   private:
    // 4KB RAM
    std::array<uint8_t, 4096> memory{};

    // variable registers
    std::array<uint8_t, 16> v{};

    std::array<uint16_t, 16> stack{};

    // Each row has 64 pixels, hence each pixel is a bit in a uint64_t
    // 64px*32px
    std::array<uint64_t, 32> display{};

    // index register
    uint16_t I{};

    // a chip-8 program starts from the memory address 0x200.
    uint16_t pc{0x200};

    // current opcode
    uint16_t opcode{};

    void executeOpCode();
};
#pragma once
#include <array>
#include <cstdint>
#include <string>

// font
namespace {

constexpr uint16_t FONT_START = 0x50;

constexpr std::array<uint8_t, 80> FONT = {
    // 0
    0xF0, 0x90, 0x90, 0x90, 0xF0,

    // 1
    0x20, 0x60, 0x20, 0x20, 0x70,

    // 2
    0xF0, 0x10, 0xF0, 0x80, 0xF0,

    // 3
    0xF0, 0x10, 0xF0, 0x10, 0xF0,

    // 4
    0x90, 0x90, 0xF0, 0x10, 0x10,

    // 5
    0xF0, 0x80, 0xF0, 0x10, 0xF0,

    // 6
    0xF0, 0x80, 0xF0, 0x90, 0xF0,

    // 7
    0xF0, 0x10, 0x20, 0x40, 0x40,

    // 8
    0xF0, 0x90, 0xF0, 0x90, 0xF0,

    // 9
    0xF0, 0x90, 0xF0, 0x10, 0xF0,

    // A
    0xF0, 0x90, 0xF0, 0x90, 0x90,

    // B
    0xE0, 0x90, 0xE0, 0x90, 0xE0,

    // C
    0xF0, 0x80, 0x80, 0x80, 0xF0,

    // D
    0xE0, 0x90, 0x90, 0x90, 0xE0,

    // E
    0xF0, 0x80, 0xF0, 0x80, 0xF0,

    // F
    0xF0, 0x80, 0xF0, 0x80, 0x80};

}  // namespace

class Chip8 {
   public:
    Chip8();
    void loadROM(const std::string& path);
    void cycle();

    const std::array<uint64_t, 32>& getDisplay() const;
    void clearDisplay();

    void setKey(uint8_t key, bool pressed);
    bool isPressed(uint8_t key) const;

    void decayDelayTimer();
    void decaySoundTimer();

   private:
    // 4KB RAM
    std::array<uint8_t, 4096> memory{};

    // variable registers
    std::array<uint8_t, 16> v{};

    // stack
    std::array<uint16_t, 16> stack{};
    uint8_t sp = 0;

    // Each row has 64 pixels, hence each pixel is a bit in a uint64_t
    // 64px*32px
    std::array<uint64_t, 32> display{};

    // index register
    uint16_t I{};

    // timers
    uint8_t delayTimer{};
    uint8_t soundTimer{};

    // keypad
    uint16_t keypad = 0;

    // a chip-8 program starts from the memory address 0x200.
    uint16_t pc{0x200};

    // current opcode
    uint16_t opcode{};

    void executeOpCode();
};
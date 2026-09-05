#include "chip8.hpp"

#include <array>
#include <cstdint>
#include <fstream>

#define GET_OPCODE() (((uint16_t)memory[pc++]) << 8 | memory[pc++])
#define ADVANCE() (pc += 2)

Chip8::Chip8() {
    // load font
    std::copy(FONT.begin(), FONT.end(), memory.begin() + FONT_START);
}

void Chip8::cycle() { executeOpCode(); }

void Chip8::loadROM(const std::string& path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);

    if (!file) throw std::runtime_error("Failed to open ROM");

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    if (size > memory.size() - 0x200)
        throw std::runtime_error("ROM is too large");

    file.read(reinterpret_cast<char*>(memory.data() + 0x200), size);
}

const std::array<uint64_t, 32>& Chip8::getDisplay() const { return display; }

void Chip8::clearDisplay() { display.fill(0); }

void Chip8::setKey(uint8_t key, bool pressed) {
    if (pressed)
        keypad |= (uint16_t{1} << key);
    else
        keypad &= ~(uint16_t{1} << key);
}

bool Chip8::isPressed(uint8_t key) const {
    return keypad & (uint16_t{1} << key);
}

void Chip8::decayDelayTimer() {
    if (delayTimer > 0) delayTimer--;
}

void Chip8::decaySoundTimer() {
    if (soundTimer > 0) soundTimer--;
}

void Chip8::executeOpCode() {
    opcode = GET_OPCODE();

    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    uint8_t n = opcode & 0x000F;
    uint8_t nn = opcode & 0x00FF;
    uint16_t nnn = opcode & 0x0FFF;

    switch (opcode & 0xF000) {
        case 0x0000:
            switch (opcode) {
                case 0x00E0:  // CLS
                    clearDisplay();
                    break;

                case 0x00EE:  // RET
                    if (sp == 0) throw std::runtime_error("Stack underflow");
                    pc = stack[--sp];
                    break;

                default:
                    /*
                        0NNN - SYS addr
                        considered a legacy/ignored instruction on modern CHIP-8
                        interpreters
                    */
                    break;
            }
            break;

        case 0x1000:  // 1NNN - JP addr
        {
            pc = nnn;
            break;
        }

        case 0x2000:  // 2NNN - CALL addr
            if (sp >= stack.size()) throw std::runtime_error("Stack overflow");
            stack[sp++] = pc;
            pc = nnn;
            break;

        case 0x3000:  // 3XNN - SE Vx, byte
            if (v[x] == nn) ADVANCE();
            break;

        case 0x4000:  // 4XNN - SNE Vx, byte
            if (v[x] != nn) ADVANCE();
            break;

        case 0x5000:  // 5XY0 - SE Vx, Vy
            // last nibble must be a zero
            if (n == 0 && v[x] == v[y]) ADVANCE();
            break;

        case 0x6000:  // 6XNN - LD Vx, byte
        {
            v[x] = nn;
            break;
        }

        case 0x7000:  // 7XNN - ADD Vx, byte
        {
            v[x] += nn;
            break;
        }

        case 0x8000:
            switch (n) {
                case 0x0:  // 8XY0 - LD Vx, Vy
                    v[x] = v[y];
                    break;

                case 0x1:  // 8XY1 - OR
                    v[x] |= v[y];
                    break;

                case 0x2:  // 8XY2 - AND
                    v[x] &= v[y];
                    break;

                case 0x3:  // 8XY3 - XOR
                    v[x] ^= v[y];
                    break;

                case 0x4:  // 8XY4 - ADD Vx, Vy
                {
                    uint16_t sum = v[x] + v[y];

                    v[0xF] = sum > 0xFF;
                    v[x] = sum & 0xFF;

                    break;
                }

                case 0x5:  // 8XY5 - SUB Vx, Vy
                    if (v[x] >= v[y])
                        v[0xf] = 1;
                    else
                        v[0xf] = 0;
                    v[x] -= v[y];
                    break;

                case 0x6:  // 8XY6 - SHR Vx
                    v[0xF] = v[x] & 1;
                    v[x] >>= 1;
                    break;

                case 0x7:  // 8XY7 - SUBN Vx, Vy
                    if (v[y] >= v[x])
                        v[0xf] = 1;
                    else
                        v[0xf] = 0;
                    v[x] = v[y] - v[x];
                    break;

                case 0xE:  // 8XYE - SHL Vx
                    v[0xF] = (v[x] & 0x80) >> 7;
                    v[x] <<= 1;
                    break;
            }
            break;

        case 0x9000:  // 9XY0 - SNE Vx, Vy
            // last nibble must be a zero
            if (n == 0 && v[x] != v[y]) ADVANCE();
            break;

        case 0xA000: {  // ANNN - LD I, addr
            I = nnn;
            break;
        }

        case 0xB000:  // BNNN - JP V0, addr
            break;

        case 0xC000:  // CXNN - RND Vx, byte
            break;

        case 0xD000: {  // DXYN - DRW Vx, Vy, nibble
            uint8_t xPos = v[x];
            uint8_t yPos = v[y];

            v[0xF] = 0;

            for (uint8_t row = 0; row < n; ++row) {
                uint8_t sprite = memory[I + row];

                uint8_t screenY = (yPos + row) % 32;

                for (uint8_t col = 0; col < 8; ++col) {
                    if ((sprite & (0x80 >> col)) == 0) continue;

                    uint8_t screenX = (xPos + col) % 64;

                    uint64_t mask = uint64_t{1} << (63 - screenX);

                    if (display[screenY] & mask) v[0xF] = 1;

                    display[screenY] ^= mask;
                }
            }

            break;
        }
        case 0xE000:
            switch (nn) {
                case 0x9E:  // EX9E - SKP Vx
                    break;

                case 0xA1:  // EXA1 - SKNP Vx
                    break;
            }
            break;

        case 0xF000:
            switch (nn) {
                case 0x07:  // FX07 - LD Vx, DT
                    break;

                case 0x0A:  // FX0A - LD Vx, K
                    break;

                case 0x15:  // FX15 - LD DT, Vx
                    break;

                case 0x18:  // FX18 - LD ST, Vx
                    break;

                case 0x1E:  // FX1E - ADD I, Vx
                    break;

                case 0x29:  // FX29 - LD F, Vx
                    break;

                case 0x33:  // FX33 - LD B, Vx
                    break;

                case 0x55:  // FX55 - LD [I], Vx
                    break;

                case 0x65:  // FX65 - LD Vx, [I]
                    break;
            }
            break;
    }
}

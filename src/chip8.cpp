#include "chip8.hpp"

#include <array>
#include <cstdint>
#include <fstream>

#define GET_OPCODE() (((uint16_t)memory[pc++]) << 8 | memory[pc++])

Chip8::Chip8() {
    // nothing to initilize yet
}

void Chip8::loadROM(const std::string& path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);

    if (!file) throw std::runtime_error("Failed to open ROM");

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    if (size > memory.size() - 0x200)
        throw std::runtime_error("ROM is too large");

    file.read(reinterpret_cast<char*>(memory.data() + 0x200), size);
}

void Chip8::executeOpCode() {
    opcode = GET_OPCODE();

    switch (opcode & 0xF000) {
        case 0x0000:
            switch (opcode) {
                case 0x00E0:  // CLS
                    // clear screen
                    break;

                case 0x00EE:  // RET
                    // return from subroutine
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
            break;

        case 0x2000:  // 2NNN - CALL addr
            break;

        case 0x3000:  // 3XNN - SE Vx, byte
            break;

        case 0x4000:  // 4XNN - SNE Vx, byte
            break;

        case 0x5000:  // 5XY0 - SE Vx, Vy
            break;

        case 0x6000:  // 6XNN - LD Vx, byte
            break;

        case 0x7000:  // 7XNN - ADD Vx, byte
            break;

        case 0x8000:
            switch (opcode & 0x000F) {
                case 0x0:  // 8XY0 - LD Vx, Vy
                    break;

                case 0x1:  // 8XY1 - OR
                    break;

                case 0x2:  // 8XY2 - AND
                    break;

                case 0x3:  // 8XY3 - XOR
                    break;

                case 0x4:  // 8XY4 - ADD Vx, Vy
                    break;

                case 0x5:  // 8XY5 - SUB Vx, Vy
                    break;

                case 0x6:  // 8XY6 - SHR Vx
                    break;

                case 0x7:  // 8XY7 - SUBN Vx, Vy
                    break;

                case 0xE:  // 8XYE - SHL Vx
                    break;
            }
            break;

        case 0x9000:  // 9XY0 - SNE Vx, Vy
            break;

        case 0xA000:  // ANNN - LD I, addr
            break;

        case 0xB000:  // BNNN - JP V0, addr
            break;

        case 0xC000:  // CXNN - RND Vx, byte
            break;

        case 0xD000:  // DXYN - DRW Vx, Vy, nibble
            break;

        case 0xE000:
            switch (opcode & 0x00FF) {
                case 0x9E:  // EX9E - SKP Vx
                    break;

                case 0xA1:  // EXA1 - SKNP Vx
                    break;
            }
            break;

        case 0xF000:
            switch (opcode & 0x00FF) {
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

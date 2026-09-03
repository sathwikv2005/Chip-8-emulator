#include "chip8.hpp"

#include <array>
#include <cstdint>
#include <fstream>

void Chip8::loadROM(const std::string& path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);

    if (!file) throw std::runtime_error("Failed to open ROM");

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    if (size > memory.size() - 0x200)
        throw std::runtime_error("ROM is too large");

    file.read(reinterpret_cast<char*>(memory.data() + 0x200), size);
}
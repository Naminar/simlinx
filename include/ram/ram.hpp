#pragma once
#include "utils/warning.h"
#include <vector>
#include <span>
#include <cstdint>

namespace simlinx {

    class Qword {
        uint8_t* world1;
        uint8_t* world2;
        uint8_t* world3;
        uint8_t* world4;

    public:
        Qword (uint8_t* w1, uint8_t* w2, uint8_t* w3, uint8_t* w4): world1{w1},
                                                                    world2{w2},
                                                                    world3{w3},
                                                                    world4{w4} {
        }

        Qword& operator = (uint64_t qworld) {
            *world1 = 0xFF & (qworld);
            *world2 = 0xFF & (qworld >> 8);
            *world3 = 0xFF & (qworld >> 16);
            *world4 = 0xFF & (qworld >> 24);
            return *this;
        }

        operator uint64_t() {
            return *world1 << 0  |
                   *world2 << 8  |
                   *world3 << 16 |
                   *world4 << 24;
        }
    };

    class RAM final {
        std::vector<uint8_t> raw_ram;
    public:
        using size_type = unsigned long long;
    public:
        RAM(size_type ram_size): raw_ram(ram_size) {}

        std::span<uint8_t> allocate_memory(size_type allocate_size) {
            BAD_IMPLEMENTED("");
            return std::span<uint8_t>(raw_ram).subspan(0, allocate_size);
        }

        uint64_t operator [](size_type index) const {
            return raw_ram[index]     << 0  |
                   raw_ram[index + 1] << 8  |
                   raw_ram[index + 2] << 16 |
                   raw_ram[index + 3] << 24;
        }

        Qword operator [](size_type index) {
            return Qword {
                &raw_ram[index],
                &raw_ram[index + 1],
                &raw_ram[index + 2],
                &raw_ram[index + 3]
            };
        }

        void store64(size_type address, uint64_t value) {
            raw_ram[address]     = 0xFF & (value);
            raw_ram[address + 1] = 0xFF & (value >> 8);
            raw_ram[address + 2] = 0xFF & (value >> 16);
            raw_ram[address + 3] = 0xFF & (value >> 24);
        }
    };
}

constexpr auto operator ""_KB(unsigned long long size) {
    return size * 1024;
}

constexpr auto operator ""_MB(unsigned long long size) {
    return size * 1024 * 1024;
}

constexpr auto operator ""_GB(unsigned long long size) {
    return size * 1024 * 1024 * 1024;
}
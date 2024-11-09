
#include <array>
#include <cstdint>
#include "instruction.hh"

class Core {
    std::array<uint64_t, 32> regs;

    void decode(uint64_t decodedBits, BasedInstruction& decodedInstr);
};
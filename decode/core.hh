
#include <array>
#include <cstdint>
#include "instruction.hh"

class Core {
    std::array<uint64_t, 32> regs;
    uint64_t pc_reg = 0U;

    void decode(uint64_t decodedBits, ISA::BasedInstruction& decodedInstr);
};
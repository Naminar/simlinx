#pragma once
#include "cpu/instruction.hh"
#include <array>
#include <cstdint>

namespace simlinx {
  struct Core final {
  public:
    using reg_t = uint64_t;

  public:
    std::array<reg_t, 32> regs;
    reg_t pc_reg = 0U;

  public:
    void decode(uint64_t decodedBits, ISA::BasedInstruction &decodedInstr);
  };
} // namespace simlinx
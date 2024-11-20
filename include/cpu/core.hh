#pragma once
#include "cpu/instruction.hh"
#include "ram/ram.hh"
#include <array>
#include <cstdint>

namespace simlinx {
  struct Core final {
  public:
    using reg_t = uint64_t;

  public:
    std::array<reg_t, 32> regs;
    reg_t pc_reg = 0U;
    RAM &mem;

  public:
    void run(reg_t pc);
    void decode(uint32_t decodedBits, ISA::BasedInstruction &decodedInstr);
    Core(RAM &ram) : mem(ram) {};
  };
} // namespace simlinx
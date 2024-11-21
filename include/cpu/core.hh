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
    Core(RAM &ram) : mem(ram){};

    enum Register {
      zero = 0,
      ra = 1,
      sp = 2,
      gp = 3,
      tp = 4,
      t0 = 5,
      t1 = 6,
      t2 = 7,
      s0 = 8,
      fp = 8,
      s1 = 9,
      a0 = 10,
      a1 = 11,
      a2 = 12,
      a3 = 13,
      a4 = 14,
      a5 = 15,
      a6 = 16,
      a7 = 17,
      s2 = 18,
      s3 = 19,
      s4 = 20,
      s5 = 21,
      s6 = 22,
      s7 = 23,
      s8 = 24,
      s9 = 25,
      s10 = 26,
      s11 = 27,
      t3 = 28,
      t4 = 29,
      t5 = 30,
      t6 = 31
    };
  };
} // namespace simlinx
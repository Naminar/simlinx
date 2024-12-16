#pragma once
#include "cache/basicBlockCache.hh"
#include "cpu/fault.hh"
#include "cpu/instruction.hh"
#include "ram/ram.hh"
#include "reg-file/csrRegFile.hh"
#include "virtual-memory/mmu.hh"
#include <array>
#include <cstdint>

namespace simlinx {
  struct Core final {
  public:
    using reg_t = uint64_t;

  public:
    CSR CSRs;
    std::array<reg_t, 32> regs;
    reg_t pc_reg = 0U;
    RAM &mem;
    MMU mmu;
    BasicBlockCache icache;
    Fault fault = Fault::NO_FAULT;
    uint64_t executedI = 0;

  public:
    void run(reg_t pc);
    void decode(uint32_t decodedBits, ISA::BasedInstruction &decodedInstr);
    Core(RAM &ram) : mem(ram), mmu(*this){};
    void dump() const {
      for (size_t i = 0; i < regs.size(); i++) {
        std::cout << "Reg" << i << ": " << regs[i] << std::endl;
      }
    }

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

#include "cache/basicBlockCacheImpl.hh"
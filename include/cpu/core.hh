#pragma once
#include "cache/basicBlockCache.hh"
#include "cpu/fault.hh"
#include "cpu/instruction.hh"
#include "ram/ram.hh"
#include "cpu/reg.hh"
#include "reg-file/csrRegFile.hh"
#include "virtual-memory/mmu.hh"
#include <array>
#include <cstdint>


struct X86JitCompiller;

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
    void decode(uint32_t decodedBits, ISA::BasedInstruction &decodedInstr, X86JitCompiller& gen);
    Core(RAM &ram) : mem(ram), mmu(*this){};
    void dump() const {
      for (size_t i = 0; i < regs.size(); i++) {
        std::cout << "Reg" << i << ": " << regs[i] << std::endl;
      }
    }
  };
} // namespace simlinx

#include "cache/basicBlockCacheImpl.hh"
#pragma once
#include "cache/fullyAssociative.hh"
#include "cpu/instruction.hh"
#include "cpu/fault.hh"
#include "jit/jitCompiller.hh"
#include <tuple>
#include <type_traits>

namespace simlinx {
  struct Core;

  using namespace ISA;
  template <uint32_t blockSize = 64>
    requires(blockSize > 0)
  class BasicBlock {
    std::array<BasedInstruction, blockSize> instructions;
    void (*f)() = nullptr;
  public:
    BasicBlock() = default;
    BasicBlock(Core &core, u_int64_t pc, X86JitCompiller& jit);
    Fault execute(Core &core);
    void dump(u_int32_t) const;
  };

  class BasicBlockCache : public FullyAssociativeCache<BasicBlock<>> {
  public:
    BasicBlock<> *createNewBlock(Core &core, X86JitCompiller& jit); // { insert(BasicBlock<>(core,
                                              // core.pc_reg), core.pc_reg); }
    BasicBlock<> *
    createNewBlock(Core &core,
                   uint64_t pc, X86JitCompiller& jit); // { insert(BasicBlock<>(core, pc), pc); }
  };
} // namespace simlinx
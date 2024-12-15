#pragma once
#include "cache/fullyAssociative.hh"
#include "cpu/instruction.hh"
#include "cpu/fault.hh"
#include <tuple>
#include <type_traits>

namespace simlinx {
  struct Core;

  using namespace ISA;
  template <uint32_t blockSize = 64>
    requires(blockSize > 0)
  class BasicBlock {
    std::array<BasedInstruction, blockSize> instructions;

  public:
    BasicBlock() = default;
    BasicBlock(Core &core, u_int64_t pc);
    Fault execute(Core &core);
    void dump() const;
  };

  class BasicBlockCache : public FullyAssociativeCache<BasicBlock<>> {
  public:
    void createNewBlock(Core &core); // { insert(BasicBlock<>(core,
                                     // core.pc_reg), core.pc_reg); }
    void createNewBlock(Core &core,
                        uint64_t pc); // { insert(BasicBlock<>(core, pc), pc); }
  };
} // namespace simlinx
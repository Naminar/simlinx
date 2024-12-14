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
    BasicBlock(Core &core);
    Fault execute(Core &core);
  };

  class BasicBlockCache : public FullyAssociativeCache<BasicBlock<>> {};
} // namespace simlinx
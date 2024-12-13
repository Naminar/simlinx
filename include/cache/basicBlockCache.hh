#pragma once
#include "cache/fullyAssociative.hh"
#include "cpu/core.hh"
#include "cpu/instruction.hh"

namespace simlinx {
  using namespace ISA;
  class BasicBlock {
    std::array<BasedInstruction, 64> instructions;

  public:
    BasicBlock(Core &core);
    void execute(Core &core);
  };

  class BasicBlockCache : public FullyAssociativeCache<BasicBlock> {};
} // namespace simlinx
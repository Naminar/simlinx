#pragma once
#include "cache/fullyAssociative.hh"
#include "cpu/core.hh"
#include "cpu/instruction.hh"
#include <tuple>
#include <type_traits>

namespace simlinx {
  using namespace ISA;
  template <uint32_t blockSize = 64>
    requires(blockSize > 0)
  class BasicBlock {
    std::array<BasedInstruction, blockSize> instructions;

  public:
    BasicBlock(Core &core) {
      auto basicBlockPC = core.pc_reg;
      uint32_t instructionsItr = 0;
      BasedInstruction decodingInst;
      do {
        auto decodedBits = core.mem.load<uint32_t>(basicBlockPC);
        core.decode(decodedBits, decodingInst);
        instructions[instructionsItr] = decodingInst;
        basicBlockPC += 4;
        instructionsItr++;
      } while (instructionsItr < blockSize - 1 && !decodingInst.isEBB());
      decodingInst.instrId = InstrId::EBBC;
      instructions[instructionsItr] = decodingInst;
    }

    Fault execute(Core &core) {
      uint32_t instructionsItr = 0;
      auto inst = instructions[instructionsItr];
      /// TODO: maybe try and catch
      Fault fault = Fault::NO_FAULT;
      while (instructionsItr < blockSize && inst.instrId != InstrId::EBBC &&
             fault == Fault::NO_FAULT) {
        fault = ISA::executeFunctions[inst.instrId](core, inst);
        instructionsItr++;
        inst = instructions[instructionsItr];
      }
      return fault;
    }
  };

  class BasicBlockCache : public FullyAssociativeCache<BasicBlock<>> {};
} // namespace simlinx
#pragma once
#include "cache/basicBlockCache.hh"
#include "cpu/core.hh"
#include "cpu/execute.gen.hh"

namespace simlinx {

    template <uint32_t blockSize> requires(blockSize > 0)
    BasicBlock<blockSize>::BasicBlock(Core &core) {
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

    template <uint32_t blockSize> requires(blockSize > 0)
    Fault BasicBlock<blockSize>::execute(Core &core) {
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
}
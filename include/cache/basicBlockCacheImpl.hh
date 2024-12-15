#pragma once
#include "cache/basicBlockCache.hh"
#include "cpu/core.hh"
#include "cpu/execute.gen.hh"

namespace simlinx {

  template <uint32_t blockSize>
    requires(blockSize > 0)
  BasicBlock<blockSize>::BasicBlock(Core &core, uint64_t pc) {
    auto basicBlockPC = pc;
    uint32_t instructionsItr = 0;
    BasedInstruction decodingInst;
    // std::cout << "Creating BasicBlock" << std::endl;
    do {
      auto decodedBits = core.mem.load<uint32_t>(basicBlockPC);
      core.decode(decodedBits, decodingInst);
      instructions[instructionsItr] = decodingInst;
      // std::cout << "PC: 0x" << std::hex << basicBlockPC << std::dec
      //           << " Decoding instruction: " << decodingInst.instrId
      //           << " EBB: " << decodingInst.isEBB() << std::endl;
      basicBlockPC += 4;
      instructionsItr++;
    } while (instructionsItr < blockSize - 1 && !decodingInst.isEBB());
    // std::cout << "End of creating BasicBlock" << std::endl;
    decodingInst.instrId = InstrId::EBBC;
    instructions[instructionsItr] = decodingInst;
  }

  template <uint32_t blockSize>
    requires(blockSize > 0)
  Fault BasicBlock<blockSize>::execute(Core &core) {
    uint32_t instructionsItr = 0;
    auto inst = instructions[instructionsItr];
    /// TODO: maybe try and catch
    Fault fault = Fault::NO_FAULT;
    decltype(core.pc_reg) prev_pc; // = core.pc_reg;

    if (inst.instrId == InstrId::NONE)
      fault = Fault::THE_END_OF_TASK;

    while (instructionsItr < blockSize && inst.instrId != InstrId::EBBC &&
           fault == Fault::NO_FAULT) {
      core.regs[0] = 0;
      core.executedI++;
      prev_pc = core.pc_reg;
      fault = ISA::executeFunctions[inst.instrId](core, inst);
      instructionsItr++;
      inst = instructions[instructionsItr];

      if (core.pc_reg == prev_pc)
        core.pc_reg += sizeof(uint32_t);
    }
    return fault;
  }

  template <uint32_t blockSize>
    requires(blockSize > 0)
  void BasicBlock<blockSize>::dump() const {
    std::cout << "BasicBlock:" << std::endl;
    for (auto inst : instructions) {
      std::cout << "Instruction:" << inst.instrId << std::endl;
    }
    std::cout << "End of BasicBlock" << std::endl;
  }
} // namespace simlinx
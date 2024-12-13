#include "cache/basicBlockCache.hh"
#include "cpu/execute.gen.hh"

namespace simlinx {
  BasicBlock::BasicBlock(Core &core) {
    auto basicBlockPC = core.pc_reg;
    uint32_t instructionsItr = 0;
    BasedInstruction decodingInst;
    do {
      auto decodedBits = core.mem.load<uint32_t>(basicBlockPC);
      core.decode(decodedBits, decodingInst);
      instructions[instructionsItr] = decodingInst;
      basicBlockPC += 4;
      instructionsItr++;
      /// TODO: should be TYPE::JUMP!!!!
    } while (instructionsItr < 63 && decodingInst.instrId != InstrId::NONE);
  }

  void BasicBlock::execute(Core &core) {
    uint32_t instructionsItr = 0;
    auto inst = instructions[instructionsItr];
    /// TODO: maybe try and catch
    while (instructionsItr < 64 && inst.instrId != InstrId::NONE) {
      auto fault = ISA::executeFunctions[inst.instrId](core, inst);
      instructionsItr++;
      inst = instructions[instructionsItr];
    }
  }
} // namespace simlinx
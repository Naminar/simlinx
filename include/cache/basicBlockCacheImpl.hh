#pragma once
#include "cache/basicBlockCache.hh"
#include "cpu/core.hh"
#include "cpu/execute.gen.hh"
#include "jit/jitCompiller.hh"
#include <sys/mman.h>
#include <cstring>
#include <unistd.h>

namespace simlinx {

  inline void* allocateExecutableMemory(size_t size) {
      size = (size + 4095) & ~4095; // Выравнивание размера по границе страницы (обычно 4KB)
      void* memory = mmap(nullptr, size, PROT_READ | PROT_WRITE | PROT_EXEC, 
                          MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

      if (memory == MAP_FAILED) {
          perror("mmap");
          return nullptr;
      }

      return memory;
  }


  template <uint32_t blockSize>
    requires(blockSize > 0)
  BasicBlock<blockSize>::BasicBlock(Core &core, uint64_t pc, X86JitCompiller& jit) {
    auto basicBlockPC = pc;
    uint32_t instructionsItr = 0;
    BasedInstruction decodingInst;
    jit.load_regs(core);
    do {
      auto decodedBits = core.mem.load<uint32_t>(basicBlockPC);
      core.decode(decodedBits, decodingInst, jit);
      instructions[instructionsItr] = decodingInst;
      basicBlockPC += 4;
      instructionsItr++;
    } while (instructionsItr < blockSize - 1 && !decodingInst.isEBB());
    std::cout << "Dump:\n";
    jit.gen.dump();
    std::cout << "Dump end\n";
    jit.save_regs(core);

    size_t size = 4096; // Выделяем одну страницу
    void* memory = allocateExecutableMemory(size);
    std::memcpy(memory, jit.gen.getCode(), jit.gen.getSize());

    terminatorIdx = instructionsItr - 1;
    f = jit.gen.getCode<void (*)()>();
    // f = (void(*)())memory;

    decodingInst.instrId = InstrId::EBBC;
    decodingInst.exec = executeEbbc;
    instructions[instructionsItr] = decodingInst;
  }

  template <uint32_t blockSize>
    requires(blockSize > 0)
  Fault BasicBlock<blockSize>::execute(Core &core, X86JitCompiller& jit) {
    auto inst = instructions[0];
    Fault fault = Fault::NO_FAULT;

    if (inst.instrId == InstrId::NONE)
      return Fault::THE_END_OF_TASK;
    auto prev_pc = core.pc_reg;

    f();
    jit.gen.reset();
    instructions[terminatorIdx].exec(core, &instructions[0], &instructions[terminatorIdx]);

    if (core.fault != Fault::NO_FAULT)
      fault = core.fault;
    return fault;
  }

  template <uint32_t blockSize>
    requires(blockSize > 0)
  void BasicBlock<blockSize>::dump(u_int32_t indx) const {
    std::cout << "BasicBlock:" << indx << std::endl;
    for (uint32_t i = 0; i < blockSize; i++) {
      std::cout << "Instruction:" << instructions[i].dumpName()
                << " addr: " << &instructions[i] << std::endl;
    }
    std::cout << "End of BasicBlock" << std::endl;
  }
} // namespace simlinx
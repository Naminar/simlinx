#include "cpu/core.hh"
#include "cpu/cpu.hh"
#include "cpu/execute.gen.hh"
#include "jit/jitCompiller.hh"
#include <bitset>
#include <chrono>
#include <iostream>
#include <print>

namespace simlinx {

  void Core::run(Core::reg_t pc) {
    pc_reg = pc;
    regs[2] = 900_KB;
    regs[0] = 0;
    BasicBlock<> *bb;
    Fault fault = Fault::NO_FAULT;

    X86JitCompiller jit;

    auto start = std::chrono::high_resolution_clock::now();
    while (true && fault == Fault::NO_FAULT && this->fault == Fault::NO_FAULT) {
      bb = icache.lookup(pc_reg);
      if (!bb)
        bb = icache.createNewBlock(*this, jit);
      fault = bb->execute(*this, jit);
    }
    std::cout << "final icache:" << std::endl;
    icache.dump();
    auto end = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    dump();
    std::cout << "Hit: " << icache.getHit() << " Miss: " << icache.getMiss()
              << std::endl;
    std::cout << "Execution time: " << duration.count() << " microseconds"
              << std::endl;
    std::cout << "Instructions executed: " << executedI << std::endl;
    std::cout << "MIPS: " << float(executedI) / float(duration.count())
              << std::endl;
  }
} // namespace simlinx
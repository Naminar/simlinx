#include "cpu/core.hh"
#include "cpu/cpu.hh"
#include "cpu/execute.gen.hh"
#include <bitset>
#include <iostream>
#include <print>

namespace simlinx {

  void Core::run(Core::reg_t pc) {
    pc_reg = pc;
    regs[2] = 900_KB;
    regs[0] = 0;
    BasicBlock<> *bb;
    Fault fault = Fault::NO_FAULT;
    while (true && fault == Fault::NO_FAULT) {
      bb = icache.lookup(pc_reg);
      if (!bb)
        bb = icache.createNewBlock(*this);
      fault = bb->execute(*this);
    }
  }
} // namespace simlinx
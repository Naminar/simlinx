#include "cpu/cpu.hh"
#include "cpu/execute.gen.hh"
#include <bitset>
#include <iostream>
#include <print>

namespace simlinx {

  void Core::run(Core::reg_t pc) {
    pc_reg = pc;
    regs[2] = 900_KB;
    while (true)
      try {
        regs[0] = 0;
        auto prev_pc = pc_reg;
        ISA::BasedInstruction inst;
        auto decodedBits = mem.load<uint32_t>(pc_reg);
        std::cout << "PC = " << std::hex << pc_reg << " " << std::dec;
        decode(decodedBits, inst);
        if (inst.instrId == InstrId::NONE) {
          std::cout << std::dec << " | InstrId = NONE" << std::endl;
          std::cout << "reg[10] " << regs[10] << "| reg[11] " << regs[11]
                    << std::endl;
          int x = 0;
          std::cout << "===========================" << std::endl;
          for (auto reg : regs) {
            std::cout << "reg [" << x << "] " << reg << std::endl;
            x += 1;
          }
          return;
        }

        try {
          auto fault = ISA::executeFunctions[inst.instrId](*this, inst);
          std::cout << std::endl;
          if (fault != Fault::NO_FAULT) {
            std::cout << "fault = " << (int) fault << std::endl;
            return;
          }
        } catch (std::exception &e) {
          std::println("executeFunctions exception! PC = {}", pc_reg);
          std::println("{}", e.what());
          return;
        }

        if (pc_reg == prev_pc)
          pc_reg += sizeof(uint32_t);
        else {
          std::cout << "jump" << std::endl;
        }
      }

      catch (std::exception &e) {
        std::println("load PC = {}", pc_reg);
        std::println("{}", e.what());
        return;
      }
  };
} // namespace simlinx
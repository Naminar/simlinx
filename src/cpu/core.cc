#include "cpu/cpu.hh"
#include "cpu/execute.gen.hh"
#include <iostream>
#include <print>
#include <bitset>

namespace simlinx {
    void Core::run(Core::reg_t pc) { 
        pc_reg = pc;
        regs[2] = 900_KB;
                
        while (true) try {
            auto prev_pc = pc_reg;
            ISA::BasedInstruction inst;
            auto decodedBits = mem.load<uint32_t>(pc_reg);
#ifdef DEBUG
            std::cout << "pc = " << pc_reg 
                      << ", raw_ram[pc] = " << mem.raw_ram[pc_reg]
                      << ", decoded bits =  " << decodedBits << "\n"; 
#endif
            decode(decodedBits, inst);
            try {
                auto fault = ISA::executeFunctions[inst.instrId](*this, inst);
                if (fault != Fault::NO_FAULT)
                    return;
            } catch  (std::exception& e) {
                std::println("executeFunctions exception! PC = {}", pc_reg);
                std::println("{}", e.what());
                return;
            }

            if (pc_reg == prev_pc)
                pc_reg += sizeof(uint32_t);
        }

        catch (std::exception& e) {
            std::println("load PC = {}", pc_reg);
            std::println("{}", e.what());
            return;
        }
    };
}
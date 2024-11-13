#include "cpu/cpu.hh"
#include "cpu/execute.gen.hh"
#include <iostream>
#include <print>

namespace simlinx {
    void Core::run(Core::reg_t pc) { 
        pc_reg = pc;
        regs[2] = 900_KB;
        printf("regs2 %b\n", regs[2]);
        for (auto byte : std::span<uint8_t>(mem.raw_ram).subspan(0, 300)) {
            std::cout << std::hex << byte << " ";
        }
        std::cout << "\n";
        
        while (true) try {
            auto prev_pc = pc_reg;
            ISA::BasedInstruction inst;
            auto decodedBits = mem.load<uint32_t>(pc_reg);
            // std::cout << "pc = " << pc_reg 
            //           << ", raw_ram[pc] = " << mem.raw_ram[pc_reg]
            //           << ", decoded bits =  " << decodedBits << "\n"; 
            decode(decodedBits, inst);
            // std::cout << inst.instrId << " " << inst.instrBits << "\n";

            try {
                ISA::executeFunctions[inst.instrId](*this, inst);
            } catch  (std::exception& e) {
                std::println("executeFunctions exception! PC = {}", pc_reg);
                std::println("{}", e.what());
                return;
            }

            if (pc_reg == prev_pc)
                pc_reg += sizeof(uint32_t);
        }

        catch (std::exception e) {
            std::println("load PC = {}", pc_reg);
            std::println("{}", e.what());
            return;
        }
    };
}
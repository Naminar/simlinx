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
            std::cout << "PC = " << std::hex <<  pc_reg << " " << std::dec;
            decode(decodedBits, inst);
            if (inst.instrId == InstrId::NONE) {                
                std::cout << "PC = " << std::hex << pc_reg << std::dec <<" | InstrId = " << inst.instrId << std::endl;
                std::cout << "reg[10] "<< regs[10] << "| reg[11] " << regs[11] << std::endl;
                int x = 0;
                for (auto reg : regs) {
                    std:: cout << x << " " << reg << std::endl;
                     x += 1;
                    }
                   
                    // std::cout << "reg[10] "<< regs[10] << "| reg[11] " << regs[11] << std::endl;
                return;
            }

            try {
                auto fault = ISA::executeFunctions[inst.instrId](*this, inst);
                if (fault != Fault::NO_FAULT) {
                    std::cout << "fault = " << fault << std::endl;
                    return;
                }
            } catch  (std::exception& e) {
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

        catch (std::exception& e) {
            std::println("load PC = {}", pc_reg);
            std::println("{}", e.what());
            return;
        }
    };
}
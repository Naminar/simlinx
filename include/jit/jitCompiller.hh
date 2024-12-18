#pragma once
#include "cpu/reg.hh"
#include "xbyak/xbyak.h"
#include <array>
#include <cassert>
#include <print>

namespace simlinx {
    struct Core;
}

struct X86JitCompiller final { 
    Xbyak::CodeGenerator gen;
    simlinx::reg_t saved_rsp = 0;
    
    const Xbyak::Reg64 tmp_64 = Xbyak::util::rbx;
    const Xbyak::Reg32 tmp_32 = Xbyak::util::ebx;
    const Xbyak::Reg16 tmp_16 = Xbyak::util::bx;
    const Xbyak::Reg8  tmp_8  = Xbyak::util::bl;
    
    const Xbyak::Reg64 zero_64 = Xbyak::util::r15;
    const Xbyak::Reg32 zero_32 = Xbyak::util::r15d;
    const Xbyak::Reg16 zero_16 = Xbyak::util::r15w;
    const Xbyak::Reg8  zero_8  = Xbyak::util::r15b;

    void load_regs(simlinx::Core& core);
    void save_regs(simlinx::Core& core);

    Xbyak::Reg64 reg_64(uint32_t riscvReg) {
        if (riscvReg == simlinx::zero) {
            gen.xor_(zero_64, zero_64);
            return zero_64;
        }
        
        if (!x86Regs64.contains(riscvReg)) {
            std::println("Register {} does not supported", riscvReg);
        }

        return x86Regs64[riscvReg];
    }

    Xbyak::Reg32 reg_32(uint32_t riscvReg) {
        if (riscvReg == simlinx::zero) {
            gen.xor_(zero_32, zero_32);
            return zero_32;
        }
        
        if (!x86Regs32.contains(riscvReg)) {
            std::println("Register {} does not supported", riscvReg);
        }

        return x86Regs32[riscvReg];
    }

    Xbyak::Reg16 reg_16(uint32_t riscvReg) {
        if (riscvReg == simlinx::zero) {
            gen.xor_(zero_16, zero_16);
            return zero_16;
        }
        
        if (!x86Regs16.contains(riscvReg)) {
            std::println("Register {} does not supported", riscvReg);
        }

        return x86Regs16[riscvReg];
    }

    Xbyak::Reg8 reg_8(uint32_t riscvReg) {
        if (riscvReg == simlinx::zero) {
            gen.xor_(zero_8, zero_8);
            return zero_8;
        }
        
        if (!x86Regs8.contains(riscvReg)) {
            std::println("Register {} does not supported", riscvReg);
        }

        return x86Regs8[riscvReg];
    }

    std::unordered_map<uint32_t, Xbyak::Reg64> x86Regs64 = {
        {simlinx::sp, Xbyak::util::rsp},
        {simlinx::ra, Xbyak::util::rax},
        {simlinx::s0, Xbyak::util::rbp},
        {simlinx::a0, Xbyak::util::rdi},
        {simlinx::a4, Xbyak::util::r8},
        {simlinx::a5, Xbyak::util::r9}
    };

    std::unordered_map<uint32_t, Xbyak::Reg32> x86Regs32 = {
        {simlinx::sp, Xbyak::util::esp},
        {simlinx::ra, Xbyak::util::eax},
        {simlinx::s0, Xbyak::util::ebp},
        {simlinx::a0, Xbyak::util::edi},
        {simlinx::a4, Xbyak::util::r8d},
        {simlinx::a5, Xbyak::util::r9d}
    };

    std::unordered_map<uint32_t, Xbyak::Reg16> x86Regs16 = {
        {simlinx::sp, Xbyak::util::sp},
        {simlinx::ra, Xbyak::util::ax},
        {simlinx::s0, Xbyak::util::bp},
        {simlinx::a0, Xbyak::util::di},
        {simlinx::a4, Xbyak::util::r8w},
        {simlinx::a5, Xbyak::util::r9w}
    };

    std::unordered_map<uint32_t, Xbyak::Reg8> x86Regs8 = {
        {simlinx::sp, Xbyak::util::spl},
        {simlinx::ra, Xbyak::util::al},
        {simlinx::s0, Xbyak::util::bpl},
        {simlinx::a0, Xbyak::util::dil},
        {simlinx::a4, Xbyak::util::r8b},
        {simlinx::a5, Xbyak::util::r9b}
    };
};
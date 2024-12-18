#include "jit/jitCompiller.hh"
#include "cpu/core.hh"

using namespace Xbyak::util;

void genCode(Xbyak::CodeGenerator& code, int x) {
    using namespace Xbyak::util;
    code.mov(eax, x);
    code.ret();
}

void X86JitCompiller::load_regs(simlinx::Core& core) {
    
    
    // try {
        // gen.mov(rbx, reinterpret_cast<uint64_t>(&saved_rsp));
    //     gen.mov(ptr[rbx], rsp);
    // } catch (std::exception& e) {
    //     std::cout << "save rsp error: " << e.what() << "\n";
    // }

    // try {
    //     gen.mov(rbx, reinterpret_cast<uint64_t>(&core.regs));
    // } catch (std::exception& e) {
    //     std::cout << "load core.regs error: " << e.what() << "\n";
    // }

    // try {
    //     gen.mov(rsp, ptr[rbx + simlinx::sp * sizeof(simlinx::reg_t)]);    
    //     gen.mov(rax, ptr[rbx + simlinx::ra * sizeof(simlinx::reg_t)]);
    //     gen.mov(rbp, ptr[rbx + simlinx::s0 * sizeof(simlinx::reg_t)]);
    //     gen.mov(rdi, ptr[rbx + simlinx::a0 * sizeof(simlinx::reg_t)]);
    //     gen.mov(r8,  ptr[rbx + simlinx::a4 * sizeof(simlinx::reg_t)]);
    //     gen.mov(r9,  ptr[rbx + simlinx::a5 * sizeof(simlinx::reg_t)]);
    // } catch (std::exception& e) {
    //     std::cout << "load regs error: " << e.what() << "\n";
    // }

    std::cout << "load_regs dump:\n";
    gen.dump();
    std::cout << "load_regs dump end\n";
}


void X86JitCompiller::save_regs(simlinx::Core& core) {
    // try {
    //     gen.mov(rbx, reinterpret_cast<uint64_t>(&core.regs));
    // } catch (std::exception& e) {
    //     std::cout << "load core.regs error: " << e.what() << "\n";
    // }

    // try {
    //     gen.mov(ptr[rbx + simlinx::sp * sizeof(simlinx::reg_t)], rsp);    
    //     gen.mov(ptr[rbx + simlinx::ra * sizeof(simlinx::reg_t)], rax);
    //     gen.mov(ptr[rbx + simlinx::s0 * sizeof(simlinx::reg_t)], rbp);
    //     gen.mov(ptr[rbx + simlinx::a0 * sizeof(simlinx::reg_t)], rdi);
    //     gen.mov(ptr[rbx + simlinx::a4 * sizeof(simlinx::reg_t)], r8);
    //     gen.mov(ptr[rbx + simlinx::a5 * sizeof(simlinx::reg_t)], r9);
    // } catch (std::exception& e) {
    //     std::cout << "save regs error: " << e.what() << "\n";
    // }    

    // try {
    //     gen.mov(rbx, reinterpret_cast<uint64_t>(&saved_rsp));
    //     gen.mov(rsp, ptr[rbx]);
    // } catch (std::exception& e) {
    //     std::cout << "load rsp error: " << e.what() << "\n";        
    // }

    gen.ret();
    std::cout << "save_regs dump:\n";
    gen.dump();
    std::cout << "save_regs dump end\n";
}

Fault executeInstr(Core& core, ISA::BasedInstruction& instr) {
    core.regs[instr.rd] = 0;
}
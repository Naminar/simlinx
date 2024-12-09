#include "cpu/cpu.hh"
#include "cpu/execute.gen.hh"
#include <bitset>
#include "spdlog/spdlog.h"
#include "spdlog/fmt/bin_to_hex.h"

namespace simlinx {

  void Core::run(Core::reg_t pc) {
    pc_reg = pc;
    regs[2] = 900_KB;
    while (true) {
      regs[0] = 0;
      auto prev_pc = pc_reg;
      ISA::BasedInstruction inst;
      auto decodedBits = mem.load<uint32_t>(pc_reg);
      SPDLOG_DEBUG("PC = {:#x}", pc_reg);

      decode(decodedBits, inst);
      if (inst.instrId == InstrId::NONE) {
        SPDLOG_DEBUG("InstrID = {}", static_cast<int>(inst.instrId));
        for (auto i = 0uz; i < regs.size(); ++i) {
          SPDLOG_DEBUG("Reg[{}] = {}", i, regs[i]);
        }
        return;
      }

      auto fault = ISA::executeFunctions[inst.instrId](*this, inst);
      if (fault != Fault::NO_FAULT) {
        SPDLOG_DEBUG("Fault: {}", static_cast<int>(fault));
        return;
      }

      if (pc_reg == prev_pc)
        pc_reg += sizeof(uint32_t);
      else {
        SPDLOG_DEBUG("jump");
      }
    }
  };
} // namespace simlinx
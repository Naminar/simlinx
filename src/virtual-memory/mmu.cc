
#include "virtual-memory/mmu.hh"

Addr simlinx::MMU::translate(Addr va, MemoryMode mode) {
  std::optional<Addr> pa;
  if (core.CSRs[CSRRegister::satp] >> 60 == 8U) {
      
    if (mode == MemoryMode::READ) {
        pa = tlbR.translate(va);
    }
    if (mode == MemoryMode::WRITE) {
        pa = tlbW.translate(va);
    }
    if (mode == MemoryMode::EXEC) {
        pa = tlbX.translate(va);
    }

    if (pa.has_value()) {
        return pa.value();
    } else {
        pa = walker.walk(va, mode);
    }
  } else 
    assert(false);

  return va;
}
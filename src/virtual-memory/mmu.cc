
#include "virtual-memory/mmu.hh"

Addr MMU::translate(Addr va, MemoryMode mode) {
  std::optional<Addr> addr;

  if (mode == MemoryMode::READ) {
    addr = tlbR.translate(va);
  }
  if (mode == MemoryMode::WRITE) {
    addr = tlbW.translate(va);
  }
  if (mode == MemoryMode::EXEC) {
    addr = tlbE.translate(va);
  }

  if (addr.has_value()) {
    return addr.value();
  } else {
    walker.walk(va);
  }
}
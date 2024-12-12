
#include "virtual-memory/mmu.hh"
#include "cpu/core.hh"

namespace simlinx {

  MMU::MMU(Core &_core) : core(_core), walker(_core, *this){};

  Addr MMU::translate(Addr va, MemoryMode mode) {
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
        Addr walk_pa;
        PageFault fault = walker.walk(va, &walk_pa, mode);
        assert(fault == PageFault::NoFault);
        return walk_pa;
      }
    } else
      assert(false);

    return va;
  }
} // namespace simlinx
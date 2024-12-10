
#pragma once

#include "virtual-memory/bitUnion.hh"
#include "virtual-memory/mmu.hh"
#include "virtual-memory/tlb.hh"
#include <cstdint>

namespace simlinx {

  struct Core;
  class MMU;

  enum class MemoryMode { READ, WRITE, EXEC };

  class PageWalker {
  private:
    Core &core;
    MMU &mmu;
    Addr rootAddr;
    const uint64_t pageSize = 4096;
    const int32_t levels = 3;
    const u_int64_t PTESize = 8;

    enum : uint64_t {
      vbit = 1 << 0,
      rbit = 1 << 1,
      wbit = 1 << 2,
      xbit = 1 << 3,
      ubit = 1 << 4,
      gbit = 1 << 5,
      abit = 1 << 6,
      dbit = 1 << 7,
    };

  public:
    PageWalker(Core &_core, MMU &_mmu); // : core(_core), mmu(_mmu),
    // rootAddr((_core.CSRs[CSRRegister::satp] & ((1ULL << 44) - 1)) *
    // pageSize){
    // };

    uint64_t walk(Addr va, MemoryMode mode);
  };

} // namespace simlinx
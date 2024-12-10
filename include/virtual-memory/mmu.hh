
#pragma once
#include "virtual-memory/tlb.hh"
#include "virtual-memory/walker.hh"
#include <cstdint>

namespace simlinx {

  class MMU {
  private:
    TLB tlbR;
    TLB tlbW;
    TLB tlbX;
    Core &core;
    friend class PageWalker;
    PageWalker walker;

  public:
    MMU(Core &_core) : core(_core), walker(_core, *this){};
    Addr translate(Addr va, MemoryMode mode);
  };
} // namespace simlinx
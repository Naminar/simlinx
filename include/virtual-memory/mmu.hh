
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
    friend class PageWalker;
    PageWalker walker;

  public:
    MMU(Core &core) : walker(core, *this){};
    Addr translate(Addr va, MemoryMode mode);
  };
} // namespace simlinx
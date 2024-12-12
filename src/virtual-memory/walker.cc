#include "virtual-memory/walker.hh"
#include "cpu/core.hh"
#include "virtual-memory/mmu.hh"

namespace simlinx {
  PageWalker::PageWalker(Core &_core, MMU &_mmu)
      : core(_core), mmu(_mmu),
        rootAddr((_core.CSRs[CSRRegister::satp] & ((1ULL << 44) - 1)) *
                 pageSize){};

  PageFault PageWalker::walk(Addr va, Addr *pa, MemoryMode mode) {
    uint64_t a = rootAddr;
    int i = levels - 1;
    uint64_t PTEAddr;
    uint64_t shift;
    uint64_t pte;

    while (1) {
      switch (i) {
      case (2):
        shift = 30;
        break;
      case (1):
        shift = 21;
        break;
      case (0):
        shift = 12;
        break;
      }

      PTEAddr = a + ((va >> shift) & ((1U << 9) - 1)) * PTESize;
      // !!!! pte = core.mem.raw_load<uint64_t>(PTEAddr);

      if ((pte & vbit) == 0)
        return PageFault::InvalidPage;

      if (((pte & rbit) == 0) && ((pte & wbit) == wbit))
        return PageFault::WButNotR;

      if (((pte & rbit) == rbit) || ((pte & xbit) == xbit))
        break;

      i = i - 1;
      if (i < 0)
        return PageFault::ILessThenZero;

      a = (pte >> 10) * pageSize;
    }

    if ((mode == MemoryMode::READ && (pte & rbit) == 0) ||
        (mode == MemoryMode::WRITE && (pte & wbit) == 0) ||
        (mode == MemoryMode::EXEC && (pte & xbit) == 0))
      return PageFault::AccessType;

    // if((i > 0) && (((pte >> 10) & 0x3ff) != 0))
    // return PageFault::SuperPage;

    if ((pte & abit) == 0)
      return PageFault::AccessBit;

    if ((mode == MemoryMode::WRITE) && ((pte & dbit) == 0))
      return PageFault::DirtyBit;

    *pa = ((pte << 2) & ~((1U << 12) - 1)) | (va & ((1U << 12) - 1));
    // if(i > 0)
    // {
    // 	pa |= va & 0x3ff000;
    // 	pa |= (uint64_t)(pte & 0xfff00000) << 2;
    // }
    // else
    // 	pa |= (uint64_t)(pte & 0xfffffc00) << 2;
    if ((pte & rbit) == rbit)
      mmu.tlbR.insert(va, *pa);
    if ((pte & wbit) == wbit)
      mmu.tlbW.insert(va, *pa);
    if ((pte & xbit) == xbit)
      mmu.tlbX.insert(va, *pa);
    return PageFault::NoFault;
  }

} // namespace simlinx
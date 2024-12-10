#pragma once
#include "mmu.hh"
#include "walker.hh"

namespace simlinx {
    uint64_t PageWalker::walk(Addr va, MemoryMode mode) {
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
          ;
        // throw PageFaultException("Page fault (invalid).\n", pte, pte_addr);

        if (((pte & rbit) == 0) && ((pte & wbit) == wbit))
          ;
        // throw PageFaultException("Page fault (W, but not R).\n", pte,
        // pte_addr);

        if (((pte & rbit) == rbit) || ((pte & xbit) == xbit))
          break;

        i = i - 1;
        if (i < 0)
          ;
        // throw PageFaultException("Page fault (i<0).\n", pte, pte_addr);

        a = (pte >> 10) * pageSize;
      }

      if ((mode == MemoryMode::READ && (pte & rbit) == 0) ||
          (mode == MemoryMode::WRITE && (pte & wbit) == 0) ||
          (mode == MemoryMode::EXEC && (pte & xbit) == 0))
        ;
      // throw PageFaultException("Page fault (access type).\n", pte, pte_addr);

      // if((i > 0) && (((pte >> 10) & 0x3ff) != 0))
      // 	throw PageFaultException("Page fault (superpage).\n", pte,
      // pte_addr);

      // if((pte & A_bit) == 0)
      // 	throw PageFaultException("Page fault (access bit).\n", pte,
      // pte_addr);

      // if((access == WRITE) && ((pte & D_bit) == 0))
      // 	throw PageFaultException("Page fault (dirty bit).\n", pte,
      // pte_addr);

      uint64_t pa = ((pte << 2) & ~((1U << 12) - 1)) | (va & ((1U << 12) - 1));
      // if(i > 0)
      // {
      // 	pa |= va & 0x3ff000;
      // 	pa |= (uint64_t)(pte & 0xfff00000) << 2;
      // }
      // else
      // 	pa |= (uint64_t)(pte & 0xfffffc00) << 2;
      if ((pte & rbit) == rbit)
        mmu.tlbR.insert(va, pa);
      if ((pte & wbit) == wbit)
        mmu.tlbW.insert(va, pa);
      if ((pte & xbit) == xbit)
        mmu.tlbX.insert(va, pa);
      return pa;
    }

}
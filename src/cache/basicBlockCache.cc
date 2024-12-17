#include "cache/basicBlockCache.hh"
#include "cpu/core.hh"
#include "xbyak/xbyak.h"

namespace simlinx {
  BasicBlock<> *BasicBlockCache::createNewBlock(Core &core, X86JitCompiller& jit) {
    insert(BasicBlock<>(core, core.pc_reg, jit), core.pc_reg);
    return getLastInserted();
  }
  BasicBlock<> *BasicBlockCache::createNewBlock(Core &core, uint64_t pc, X86JitCompiller& jit) {
    insert(BasicBlock<>(core, pc, jit), pc);
    return getLastInserted();
  }
} // namespace simlinx
#include "cache/basicBlockCache.hh"
#include "cpu/core.hh"

namespace simlinx {
  BasicBlock<> *BasicBlockCache::createNewBlock(Core &core) {
    insert(BasicBlock<>(core, core.pc_reg), core.pc_reg);
    return getLastInserted();
  }
  BasicBlock<> *BasicBlockCache::createNewBlock(Core &core, uint64_t pc) {
    insert(BasicBlock<>(core, pc), pc);
    return getLastInserted();
  }
} // namespace simlinx
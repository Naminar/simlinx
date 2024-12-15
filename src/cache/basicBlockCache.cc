#include "cache/basicBlockCache.hh"
#include "cpu/core.hh"

namespace simlinx {
  void BasicBlockCache::createNewBlock(Core &core) {
    insert(BasicBlock<>(core, core.pc_reg), core.pc_reg);
  }
  void BasicBlockCache::createNewBlock(Core &core, uint64_t pc) {
    insert(BasicBlock<>(core, pc), pc);
  }
} // namespace simlinx

#include "stat/statistic.hh"
#include <array>
#include <cstdint>
#include <iostream>
#include <unordered_map>

template <typename StorageT, uint64_t Size = 32, typename KeyT = uint64_t,
          uint64_t Alignas = 64>
class FullyAssociativeCache : public Statistic {
private:
  alignas(Alignas) std::array<StorageT, Size> alignedArray;
  std::unordered_map<KeyT, uint64_t> indexes;
  uint64_t roundRobinInd = 0;

public:
  //__builtin_prefetch
  void insert(StorageT value, KeyT key) {
    alignedArray[roundRobinInd] = value;
    indexes[key] = roundRobinInd;
    roundRobinInd = (roundRobinInd + 1) % Size;
  }
  StorageT *lookup(KeyT key) {
    if (indexes.find(key) != indexes.end()) {
      hit++;
      return &alignedArray[indexes[key]];
    }
    miss++;
    return nullptr;
  }
};
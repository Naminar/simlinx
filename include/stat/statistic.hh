
#include <cstdint>

class Statistic {
public:
  uint64_t hit = 0;
  uint64_t miss = 0;

public:
  uint64_t getHit() { return hit; };
  uint64_t getMiss() { return miss; };
};
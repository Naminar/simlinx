#pragma once
#include <stdint.h>

namespace simlinx {

  template <uint32_t N, typename T> class BaseRegFile {
    using reg_t = uint64_t;

    std::array<reg_t, N> regs;

  public:
    reg_t &operator[](T index) { return regs[index]; }
    reg_t &operator[](reg_t index) { return regs[index]; }
  };

} // namespace simlinx
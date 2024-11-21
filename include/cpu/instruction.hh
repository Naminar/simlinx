#pragma once
#include "cpu/enum.gen.hh"
#include <assert.h>
#include <bitset>
#include <cassert>
#include <concepts>
#include <cstdint>
#include <iostream>
#include <vector>

namespace ISA {

  inline int64_t asSigned(uint64_t val) { return static_cast<int64_t>(val); }
  template <std::signed_integral S = int64_t, typename U = uint64_t>
  inline S asSigned(U val) {
    return static_cast<S>(val);
  }

  constexpr inline uint64_t mask(unsigned nbits) {
    return (nbits >= 64) ? (uint64_t)-1LL : (1ULL << nbits) - 1;
  }

  template <class T>
  constexpr inline T bitsFrom(T val, unsigned ms_bit, unsigned ls_bit) {
    assert(ms_bit >= ls_bit);
    int nbits = ms_bit - ls_bit + 1;
    return (val >> ls_bit) & mask(nbits);
  }

  template <int N> constexpr inline uint64_t sext(uint64_t val) {
    bool sign_bit = bitsFrom(val, N - 1, N - 1);
    if (sign_bit)
      val |= ~mask(N);
    return val;
  }

  class BasedInstruction {
  public:
    uint64_t instrBits;

    // for array of exec functions
    InstrId instrId;

    // instruction's operands
  public:
    uint64_t offset = 0;
    uint64_t rd = 0;
    uint64_t rs1 = 0;
    uint64_t rs2 = 0;
    uint64_t rs3 = 0;
    uint64_t csr = 0;
    uint64_t imm = 0;
    uint64_t op1 = 0;
    uint64_t op2 = 0;

    void reset() {
      instrBits = 0;
      instrId = InstrId::NONE;

      offset = 0;
      rd = 0;
      rs1 = 0;
      rs2 = 0;
      rs3 = 0;
      csr = 0;
      imm = 0;
    }
    void matchBitsId(uint64_t bits, InstrId id) {
      instrBits = bits;
      instrId = id;
    }
    template <bool enBinary = true, bool enHex = true, typename T = uint64_t>
    void _dump(T param, const std::string &name) {
      std::cout << name << " = " << std::dec << param;
      if (enBinary)
        std::cout << " | binary = " << std::bitset<sizeof(T) * 8>(param);
      if (enHex)
        std::cout << " | hex = " << std::hex << param << std::endl;
      else
        std::cout << std::endl;
      std::cout << std::dec;
    }
    void dump() {
      _dump(offset, "offset");
      _dump(rd, "rd");
      _dump(rs1, "rs1");
      _dump(rs2, "rs2");
      _dump(rs3, "rs3");
      _dump(imm, "imm");
    }
  };

} // end namespace ISA
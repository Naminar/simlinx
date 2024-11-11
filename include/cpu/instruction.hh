#pragma once
#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>
#include <assert.h>
#include "cpu/enum.gen.hh"

namespace ISA {

int64_t 
asSigned(uint64_t val) {
    return static_cast<int64_t>(val);
}

constexpr uint64_t
mask(unsigned nbits)
{
    return (nbits >= 64) ? (uint64_t)-1LL : (1ULL << nbits) - 1;
}

template <class T>
constexpr T
bitsFrom(T val, unsigned ms_bit, unsigned ls_bit)
{
    assert(ms_bit >= ls_bit);
    int nbits = ms_bit - ls_bit + 1;
    return (val >> ls_bit) & mask(nbits);
}

template <int N>
constexpr uint64_t
sext(uint64_t val)
{
    bool sign_bit = bitsFrom(val, N - 1, N - 1);
    if (sign_bit)
        val |= ~mask(N);
    return val;
}

class BasedInstruction {
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
    // virtual void execute(){};

    void matchBitsId(uint64_t bits, InstrId id) {
        instrBits   = bits;
        instrId     = id;
    }
};

} // end namespace ISA
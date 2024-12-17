#include "cpu/instruction.hh"

namespace ISA {
  void BasedInstruction::reset() {
    instrBits = 0;
    instrId = InstrId::NONE;

    offset = 0;
    rd = 0;
    rs1 = 0;
    rs2 = 0;
    csr = 0;
    imm = 0;
  }

  void BasedInstruction::matchBitsId(uint64_t bits, InstrId id) {
    instrBits = bits;
    instrId = id;
  }

  template <bool enBinary, bool enHex, typename T>
  void BasedInstruction::_dump(T param, const std::string &name) {
    std::cout << name << " = " << std::dec << param;
    if (enBinary)
      std::cout << " | binary = " << std::bitset<sizeof(T) * 8>(param);
    if (enHex)
      std::cout << " | hex = " << std::hex << param << std::endl;
    else
      std::cout << std::endl;
    std::cout << std::dec;
  }
  void BasedInstruction::dump() {
    _dump(offset, "offset");
    _dump(rd, "rd");
    _dump(rs1, "rs1");
    _dump(rs2, "rs2");
    _dump(imm, "imm");
  }

  const char *BasedInstruction::dumpName() const { return InstrNames[instrId]; }

} // namespace ISA
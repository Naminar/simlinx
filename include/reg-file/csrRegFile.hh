#pragma once
#include "baseRegFile.hh"

namespace simlinx {

  namespace CSRRegister {
    enum CSRRegisterEnum {
      // Supervisor Protection and Translation
      satp = 0x180,
      // ----- Machine Trap Setup
      mstatus = 0x300,
      misa = 0x301,
      medeleg = 0x302,
      mideleg = 0x303,
      mie = 0x304,
      mtvec = 0x305,
      mcounteren = 0x306,
      mstatush = 0x310,
      // ----- Machine Trap Handling
      mscratch = 0x340,
      mepc = 0x341,
      mcause = 0x342,
      mtval = 0x343,
      mip = 0x344,
      mtinst = 0x34A,
      mtval2 = 0x34B,
      // ----- Machine Memory Protection
      pmpcfg0 = 0x3A0,
      // ...
      pmpaddr0 = 0x3B0,
      // ...
      // ----- Machine Information
      mvendorid = 0xF11,
      marchid = 0xF12,
      mimpid = 0xF13,
      mhartid = 0xF14
    };
  }

  class CSR : public BaseRegFile<4096, CSRRegister::CSRRegisterEnum> {};

} // namespace simlinx
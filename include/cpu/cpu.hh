#pragma once
#include "cpu/core.hh"
#include "ram/ram.hh"
#include "utils/warning.h"

namespace simlinx {
  class CPU final {
    Core m_core;

  public:
    RAM m_ram;

  public:
    CPU(RAM::size_type ram_size) : m_ram(ram_size) {}

    void run(Core::reg_t pc) { NOT_IMPLEMENTED(); };
  };
} // namespace simlinx
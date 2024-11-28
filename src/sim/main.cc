#include "cpu/cpu.hh"
#include <LIEF/ELF.hpp>
#include <exception>
#include <iostream>
#include <print>
#include "spdlog/spdlog.h"
#include "spdlog/fmt/bin_to_hex.h"

int main(int argc, char const *argv[]) try {
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " <riscv elf file>" << std::endl;
    return 0;
  }

  if constexpr (SPDLOG_ACTIVE_LEVEL == SPDLOG_LEVEL_DEBUG) {
    spdlog::set_level(spdlog::level::debug);
  }

  auto binary = LIEF::ELF::Parser::parse(argv[1]);

  simlinx::CPU cpu{1_MB};
  for (auto &&S : binary->sections()) {
    auto content = S.content();
    auto mem_for_segment = cpu.m_ram.get_memory(S.size(), S.virtual_address());
    std::copy(content.begin(), content.end(), mem_for_segment.begin());

    SPDLOG_DEBUG("Parse section {}", S.name());
    SPDLOG_DEBUG("Content size: {}", content.size());
    SPDLOG_DEBUG("Section size: {}", S.size());
    SPDLOG_DEBUG("Section vaddr: {}", S.virtual_address());
    SPDLOG_DEBUG("Content: {}", spdlog::to_hex(content.begin(), content.end()));
  }

  SPDLOG_DEBUG("Entry point: {}", binary->entrypoint());
  cpu.run(binary->entrypoint());
}

catch (std::exception &exc) {
  std::cout << "main: ";
  std::cout << exc.what() << std::endl;
}

#include "cpu/cpu.hh"
#include <LIEF/ELF.hpp>
#include <exception>
#include <iostream>
#include <print>

int main(int argc, char const *argv[]) try {
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " <riscv elf file>" << std::endl;
    return 0;
  }

  auto binary = LIEF::ELF::Parser::parse(argv[1]);

  simlinx::CPU cpu{3_GB};
  for (auto &&S : binary->sections()) {
    // std::println("{}", S.name());
    auto content = S.content();
    // std::println("Content size {}", content.size());
    // std::println("Section size {}", S.size());
    // std::println("Section vaddr {}", S.virtual_address());
    // std::cout << std::hex;
    // std::copy(content.begin(), content.end(),
    //           std::ostream_iterator<uint8_t>(std::cout));
    // std::cout << std::dec;
    // std::cout << "\n";

    auto mem_for_segment = cpu.m_ram.get_memory(S.size(), S.virtual_address());
    std::copy(content.begin(), content.end(), mem_for_segment.begin());
  }
  cpu.run(binary->entrypoint());
}

catch (std::exception &exc) {
  std::cout << "main: ";
  std::cout << exc.what() << std::endl;
}

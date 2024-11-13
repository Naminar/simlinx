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

  simlinx::CPU cpu{1_MB};
  const LIEF::ELF::Section* text_section = binary->get_section(".text");
  if (text_section) {
      auto content = text_section->content();
      std::println("Content size {}", content.size());
      auto mem_for_segment = cpu.m_ram.get_memory(text_section->size(), text_section->virtual_address());
      std::copy(content.begin(), content.end(), mem_for_segment.begin());
  }
  cpu.run(binary->entrypoint());
}

catch (std::exception& exc) {
  std::cout << "main: ";
  std::cout << exc.what() << std::endl;
}

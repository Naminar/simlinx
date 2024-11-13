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
      // Process content as needed
      // for (auto byte : content) {
      //  std::cout << std::hex << byte;
      // }

      auto mem_for_segment = cpu.m_ram.get_memory(text_section->size(), text_section->virtual_address());
      std::copy(content.begin(), content.end(), mem_for_segment.begin());
  }

  // for (auto S : binary->sections()) {
  //   std::cout << "virtual address = " << std::hex << S.virtual_address()
  //             << ", virtual size = " <<  S.size()
  //             << ", real size = " << S.content().size() 
  //             << ", vaddr + vsize = " << S.virtual_address() + S.size() << "\n";
  //   auto mem_for_segment = cpu.m_ram.get_memory(S.size(), S.virtual_address());
  //   std::copy(S.content().begin(), S.content().begin(), mem_for_segment.begin());

  //   for (auto byte : S.content()) {
  //     std::cout << std::hex << byte;
  //   }

  //   for (auto byte : std::span<uint8_t>(cpu.m_ram.raw_ram).subspan(S.virtual_address(), S.size())){
  //     std::cout << std::hex << byte;
  //   }
  //   std::cout << "\n";
  // }

  // for (auto S : binary->segments()) {
  //   std::cout << "virtual address = " << std::hex << S.virtual_address()
  //             << ", virtual size = " <<  S.size()
  //             << ", real size = " << S.content().size() 
  //             << ", vaddr + vsize = " << S.virtual_address() + S.size() << "\n";
  //   auto mem_for_segment = cpu.m_ram.get_memory(S.size(), S.virtual_address());
  //   std::copy(S.content().begin(), S.content().begin(), mem_for_segment.begin());

  //   for (auto byte : S.content()) {
  //     std::cout << std::hex << byte;
  //   }

  //   for (auto byte : std::span<uint8_t>(cpu.m_ram.raw_ram).subspan(S.virtual_address(), S.size())){
  //     std::cout << std::hex << byte;
  //   }
  //   std::cout << "\n";
  // }

  cpu.run(binary->entrypoint());
}

catch (std::exception exc) {
  std::cout << "main: ";
  std::cout << exc.what() << std::endl;
}

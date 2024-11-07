#include <LIEF/LIEF.hpp>
#include <iostream>

int main(int argc, char** argv) {
  // ELF
  if (std::unique_ptr<const LIEF::ELF::Binary> elf = LIEF::ELF::Parser::parse("/bin/ls")) {
    for (const LIEF::ELF::Section& section : elf->sections()) {
      std::cout << section.name() << ' ' << section.virtual_address() << '\n';
    }
  }
  return 0;
}

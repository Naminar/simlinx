#include "elf-parser/parser.hpp"
#include <LIEF/ELF.hpp>
#include <iostream>

namespace simlinx {
using namespace LIEF::ELF;

void ElfParser::parse_file(const char *filename) {
  auto binary = Parser::parse(filename);
  std::cout << binary->header();
}
} // namespace simlinx
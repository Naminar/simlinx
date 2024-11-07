#include "elf-parser/parser.hpp"

int main(int argc, char const *argv[])
{
    simlinx::ElfParser p;
    p.parse_file("/home/amadey/projects/simlinx/riscv-examples/bin/fact");
    return 0;
}

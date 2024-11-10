#include "cpu/cpu.hpp"
#include "elf-parser/parser.hpp"
#include <LIEF/ELF.hpp>
#include <print>
#include <exception>

int main(int argc, char const *argv[]) try
{
    if (argc != 2) {
        std::println("Usage: {} <riscv elf file>", argv[0]);
        return 0;
    }

    auto binary = LIEF::ELF::Parser::parse(argv[1]);
    auto raw_binary = binary->raw();

    simlinx::CPU cpu {1_GB};
    auto mem_for_file = cpu.m_ram.allocate_memory(raw_binary.size());
    std::copy(raw_binary.begin(), raw_binary.end(), mem_for_file.begin());

    cpu.run(binary->entrypoint());
}

catch (std::exception exc) {
    std::print("{}", exc.what());
}

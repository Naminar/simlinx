from jinja2 import Template

execute_cc_tmpl = Template("""
  #include "cpu/execute.gen.hh"
  #include "cpu/core.hh"

  namespace simlinx {
    struct Core;
  }

  namespace ISA {
    using reg_t = uint64_t;

    {% for instruction in implInstrSet %}
    Fault execute{{ instruction.instruction }}({{ instruction.unusedCore }}simlinx::Core& core, {{ instruction.unusedBasedInstr }}ISA::BasedInstruction& instr) {
      {{ instruction.execute | indent(2)}}
      {% if instruction.isEBB %} instr.setEBB();{% endif %}
      return Fault::NO_FAULT;
    };
    {% endfor %}
  }
""")

execute_hh_tmpl = Template("""
  #pragma once
  #include "cpu/fault.hh"
  #include "cpu/instruction.hh"
  #include "syscall/syscall.gen.hh"
  namespace ISA {
    {% for instruction in implInstrSet %}
    Fault execute{{ instruction.instruction }}(simlinx::Core& core, ISA::BasedInstruction& instr);
    {% endfor %}

    static const std::array<Fault(*)(simlinx::Core&, ISA::BasedInstruction&), {{ implInstrSet|length }}> executeFunctions = { 
    {% for instruction in implInstrSet %}  &execute{{ instruction.instruction }}{{ ", " if not loop.last else ""}}
    {% endfor %}};
  }
""")

enum_hh_tmpl = Template("""
enum InstrId {
{% for instruction in implInstrSet %}{{ instruction.instruction.upper() }} = {{ loop.index0 }},
{% endfor %} EBBC, //basic block end canary
NONE
};
""")


decoder_block_tmpl = Template(
"""{{' '*tab}}decodedInstr.matchBitsId(decodedBits, InstrId::{{ instr_id }});
{{' '*tab}}{{ decode | indent(tab)}}
""")

bitfields_hh_tmpl = Template("""
#pragma once
{% for field in fields %}
#define {{ field[0].upper() + ' '*(13 - field[0] | length) }} bitsFrom(decodedBits,{{ "%4s" | format(field[1]) }},{{ "%4s" | format(field[2]) }}){% endfor %}
""")

undef_bitfields_tmpl = Template("""
{% for field in fields %}
#undef {{ field[0].upper() }}{% endfor %}
""")
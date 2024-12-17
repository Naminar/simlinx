from jinja2 import Template

execute_cc_tmpl = Template("""
  #include "cpu/execute.gen.hh"
  #include "cpu/core.hh"

  namespace simlinx {
    struct Core;
  }

  namespace ISA {
    using reg_t = uint64_t;
    #define rs1 ((curI->rs1))
    #define rs2 ((curI->rs2))
    #define rd ((curI->rd))
    #define csr ((curI->csr))
    #define imm ((curI->imm))

    {% for instruction in implInstrSet %}
    void execute{{ instruction.instruction }}(simlinx::Core &core, BasedInstruction* bbsI, BasedInstruction* curI) {
      core.regs[0] = 0;
      {% if instruction.updateCoreState %} 
        core.pc_reg += (curI-bbsI)*sizeof(uint32_t); 
        core.executedI += (curI-bbsI+1); 
      {% endif %}
      {#{% if instruction.isEBB %} core.pc_reg += (curI-bbsI)*sizeof(uint32_t);  {% endif %}#}
      {{ instruction.execute | indent(2)}}
      
      {% if not instruction.isEBB %} (curI+1)->exec(core, 
      {% if instruction.updateCoreState %} curI {% else %} bbsI {% endif %}
      , curI+1);{% endif %}
    };
    {% endfor %}
                           
    void executeEbbc(simlinx::Core &core, BasedInstruction* bbsI, BasedInstruction* curI) {
      core.pc_reg += (curI-bbsI)*sizeof(uint32_t);
      core.executedI += (curI-bbsI);
    }
    #undef rs1
    #undef rs2
    #undef rd
    #undef csr
    #undef imm
    }
""")

execute_hh_tmpl = Template("""
  #pragma once
  #include "cpu/fault.hh"
  #include "cpu/instruction.hh"
  #include "syscall/syscall.gen.hh"
  namespace ISA {
    {% for instruction in implInstrSet %}
    void execute{{ instruction.instruction }}(simlinx::Core &core, BasedInstruction* bbsI, BasedInstruction* curI);
    {% endfor %}

    void executeEbbc(simlinx::Core &core, BasedInstruction* bbsI, BasedInstruction* curI);
    {#
    static const std::array<void(*)(simlinx::Core&, BasedInstruction*, BasedInstruction*), {{ implInstrSet|length }}> executeFunctions = { 
    {% for instruction in implInstrSet %}  &execute{{ instruction.instruction }}{{ ", " if not loop.last else ""}}
    {% endfor %}};
    #}
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
decodedInstr.exec = {{'execute'+instr_id[0].upper()+instr_id[1:].lower()}};
{% if isEBB %} decodedInstr.setEBB();{% endif %}
{{JIT}}
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
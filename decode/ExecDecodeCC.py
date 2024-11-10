import json

execute_cc = """
Fault execute{{}}(Core& core, ISA::BasedInstruction& instr)
"""

with open('ExecDecode.isa', 'r', encoding='utf-8') as f:
    data = f.read().replace('\n', '')
    data = json.loads(data)
    print(data)
import json
import csv

decoder_tree_header = """
void Core::decode(uint64_t decodedBits, BasedInstruction& decodedInstr) {
"""

execute_cc = """
Fault execute{{}}(Core& core, ISA::BasedInstruction& instr) {
"""

class Generator:
    gap=0
    out=''
    pointer=0
    tab=4

    def __init__(self, text: list, json_describtion: list):
        self.txt = text
        self.isa = dict()
        self.enum = []
        for file in json_describtion:
            self.isa.update(json.load(open(file, 'r', encoding="utf-8")))
        
        with open('ExecDecode.isa', 'r', encoding='utf-8') as f:
            self.decodeExec = f.read().replace('\n', '')
            self.decodeExec = json.loads(self.decodeExec)


    def get_token(self, who:str='noone')->str:
        token = self.txt[self.pointer] if self.pointer < len(self.txt) else None
        self.pointer += 1
        print(f'{who}:{token}')
        return token

    def return_token(self)->None:
        if self.pointer > 0:
            self.pointer -= 1

    def ID(self)->str|None:
        instr = self.get_token('id')
        if instr.lower() in self.isa:
            self.enum.append(f'{instr}')
            # return (' '*(self.gap+self.tab) + instr + '\n')
            put = self.decodeExec[instr.lower()]['decode'] if 'decode' in self.decodeExec[instr.lower()] else ''
            return (' '*(self.gap+self.tab) + f'decodedInstr.matchBitsId(decodedBits, InstrId::{instr});'
                     + f'\n{put}\n')
        else:
            return None

    
    def decode(self)->None:        
        token = self.get_token('decode')
        if token[:7] == 'decode(' and token[-1] == ')':
            if self.get_token('decode') == '{':
                token = token[7:-1]
                token =token.split('-')

                mask = None
                if isinstance(token, list):
                    if len(token) > 1:
                        num = int(token[1]) - int(token[0]) + 1
                        # mask = '1'*num + '0'*int(token[0])
                        mask = '1'*num + f'U << {int(token[0])}'
                    else:
                        # mask = '1' + '0'*int(token[0])
                        mask = '1' + f'U << {int(token[0])}'

                switch_arg = f'decodedBits & (0b{mask})'
                out_line = ' '*self.gap + f'switch({switch_arg}) {{\n'

                self.gap += self.tab
                case = self.case()
                
                token = self.get_token('decode')
                if token == '}':
                    self.gap -= self.tab
                    if case:
                        out_line += case
                        out_line += ' '*(self.gap+self.tab) + 'default: {break;}\n'
                        out_line += ' '*self.gap + f'}}\n'
                    else:
                        out_line = ''
                return (True, out_line)
            else:
                raise ValueError
        else:
            self.return_token()
            return None, ''


    def case(self)->None:
        out_line = ''
        while True:
            token = self.get_token('case')
            if token[-1] == ':':
                token = token[:-1]
                self.gap += self.tab
                decode = self.decode()
                self.gap -= self.tab
                if not decode[0]:
                    id = self.ID()
                    if id:
                        out_line += ' '*self.gap + f'case 0{token}: {{\n'\
                                    + id\
                                    + ' '*(self.gap+self.tab) + f'break;\n' + ' '*self.gap + '}\n'
                else:
                    if decode[1]:
                        out_line += ' '*self.gap + f'case 0{token}: {{\n'\
                                    + decode[1]\
                                    + ' '*(self.gap+self.tab) + f'break;\n' + ' '*self.gap + '}\n'
            else:
                self.return_token()
                break
        return out_line

    def make_decode_tree(self)->None:
        with open('decode_tree_auto.cc', 'w', encoding='utf-8') as f:
            f.write(decoder_tree_header + self.out + '}\n')

    def make_enum(self)->None:
        with open('enum_auto.hh', 'w', encoding='utf-8') as f: 
            enum_str = 'enum InstrId {\n'
            enum_str += f'  NONE,\n'
            for _ in self.enum:
                enum_str += f'  {_},\n'
            enum_str = enum_str[:-2] + '\n};\n'
            f.write(enum_str)
    
    def make_bitfields(self)->None:
        with open('bitfields_auto.hh', 'w', encoding='utf-8') as w_file:
            with open('arg_lut.csv', 'r') as r_file:
                reader = csv.reader(r_file)
                for row in reader:
                    w_file.write(f'#define {row[0].upper()} \t bitsFrom(decodedBits, {row[1]}, {row[2]})\n')


    def make_execute(self)->None:
        with open('execute_auto.cc', 'w', encoding='utf-8') as f:
            for instr in self.enum:
                put = self.decodeExec[instr.lower()]['execute'] if 'execute' in self.decodeExec[instr.lower()] else ''
                if put:
                    put += '\n  return Fault::NoFault'
                else:
                    put = '  return Fault::NOT_IMPLEMENTED'
                f.write(execute_cc.replace('{{}}', instr[0]+instr.lower()[1:]) 
                        + f'\n{put};\n}};\n'
                        )

    def start(self)->None:
        _, self.out = self.decode()
        self.make_decode_tree()
        self.make_enum()
        self.make_bitfields()
        self.make_execute()


if __name__ == '__main__':
    text = []
    with open('decoderTree.isa', 'r', encoding="utf-8") as f:
        for line in f:
            line = line.split()
            if line:
                text  = text + line
    gen = Generator(text, json_describtion=['instr_dict_rv_i.json'])
    gen.gap = gen.tab
    gen.start()

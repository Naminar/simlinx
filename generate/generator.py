import argparse
import json
import csv
import os

decoder_tree_header = """
using ISA::bitsFrom;
void simlinx::Core::decode(uint32_t decodedBits, ISA::BasedInstruction& decodedInstr) {
"""

execute_cc = """
Fault execute{{}}({{}}simlinx::Core& core, {{}}ISA::BasedInstruction& instr) {
"""

maybe_unused = "[[maybe_unused]]"

class Generator:
    gap=0
    out=''
    pointer=0
    tab=4

    hereDir = os.getcwd()
    cpuDir = 'cpu'
    cpuDirCC = f'../src/{cpuDir}/'
    cpuDirHH = f'../include/{cpuDir}/'

    def __init__(self, text: list, args, json_describtion: list):
        self.txt = text
        self.genArgs = args
        self.isa = dict()
        self.enum = []
        for file in json_describtion:
            self.isa.update(json.load(open(file, 'r', encoding="utf-8")))

        with open('execDecode.isa', 'r', encoding='utf-8') as f:
            self.decodeExec = f.read().replace('\n', '')
            self.decodeExec = json.loads(self.decodeExec)


    def get_token(self, who:str='noone')->str:
        token = self.txt[self.pointer] if self.pointer < len(self.txt) else None
        self.pointer += 1
        # print(f'{who}:{token}')
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
                token = token.split('-')

                mask = None
                if isinstance(token, list):
                    if len(token) > 1:
                        switch_arg = f'(bitsFrom(decodedBits, {token[1]}, {token[0]}))'
                        # num = int(token[1]) - int(token[0]) + 1
                        # # mask = '1'*num + '0'*int(token[0])
                        # mask = '1'*num + f'U << {int(token[0])}'
                    else:
                        switch_arg = f'(bitsFrom(decodedBits, {token[0]}, {token[0]}))'
                        # # mask = '1' + '0'*int(token[0])
                        # mask = '1' + f'U << {int(token[0])}'

                # switch_arg = f'(decodedBits & (0b{mask})) >> {int(token[0])}'
                out_line = ' '*self.gap + f'switch({switch_arg}) {{\n'

                self.gap += self.tab
                case = self.case()

                token = self.get_token('decode')
                if token == '}':
                    self.gap -= self.tab
                    if case:
                        out_line += case
                        out_line += ' '*(self.gap+self.tab) + 'default: {decodedInstr.matchBitsId(decodedBits, InstrId::NONE);}\n'
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
        with open(self.cpuDirCC+'decodeTree.gen.cc', 'w', encoding='utf-8') as f:
            f.write(self.make_header('decodeTree', 'cpu', cpu=['core', 'bitfields.gen']))
            f.write(decoder_tree_header + self.out + '}\n')

    def make_enum(self)->None:
        with open(self.cpuDirHH+'enum.gen.hh', 'w', encoding='utf-8') as f:
            enum_str = 'enum InstrId {\n'
            none_ind = 0
            for ind, name in enumerate(self.enum):
                enum_str += f'  {name} = {ind},\n'
                none_ind = ind
            none_ind += 1
            # print(none_ind)
            enum_str += f'  NONE = {none_ind}'
            enum_str = enum_str + '\n};\n'
            f.write(enum_str)

    def make_bitfields(self)->None:
        with open(self.cpuDirHH+'bitfields.gen.hh', 'w', encoding='utf-8') as w_file:
            with open('arg_lut.csv', 'r') as r_file:
                reader = csv.reader(r_file)
                for row in reader:
                    w_file.write(f'#define {row[0].upper()} \t bitsFrom(decodedBits, {row[1]}, {row[2]})\n')


    def make_execute_array_in_header(self)->None:
        with open('../include/cpu/execute.gen.hh', 'a', encoding='utf-8') as f:
            f.write('namespace ISA {\n')
            numFunctions = 0
            for instr in self.enum:
                numFunctions += 1
                f.write(execute_cc.replace('{{}}', instr[0]+instr.lower()[1:], 1)[:-2].replace('{{}}', '') + ';\n')
            f.write(f'static const std::array<Fault(*)(simlinx::Core&, ISA::BasedInstruction&), {numFunctions}> executeFunctions = {{ \n') #nullptr,\n')
            for instr in self.enum:
                f.write('&execute{{}},\n'.replace('{{}}', instr[0]+instr.lower()[1:]))
            f.write('};\n}\n')



    def make_execute(self)->None:

        with open(self.cpuDirCC+'execute.gen.cc', 'w', encoding='utf-8') as f:
            # f.write(f'#include "{self.cpuDir}/execute.gen.hh"\n')
            f.write(self.make_header('execute', 'cpu', cpu=['fault', 'core','instruction']))
            self.make_execute_array_in_header()
            f.write('\nnamespace ISA {\n')
            for instr in self.enum:
                put = self.decodeExec[instr.lower()]['execute'] if 'execute' in self.decodeExec[instr.lower()] else ''
                if put:
                    put += '\n  return Fault::NO_FAULT'
                else:
                    put = '  return Fault::NOT_IMPLEMENTED'
                instr_debug = ' std::cout << __PRETTY_FUNCTION__ << std::endl;\n instr.dump();' if self.genArgs.debug else ''
                maybe_unused_attribute = maybe_unused if 'NOT_IMPLEMENTED' in put else ''
                f.write(execute_cc.replace('{{}}', instr[0]+instr.lower()[1:], 1).replace('{{}}', maybe_unused_attribute)
                        + instr_debug
                        + f'\n{put};\n}};\n'
                        )
            f.write('}\n')

    def make_header(self, file_name:str, dirHH:str, path=os.getcwd(), **args)->str:
        """
        Creates a header file at the specified directory (dirHH) with the given filename
        (file_name), and includes all the header files specified in the argument list.
        The argument list is a dictionary where the key is the directory to include the
        file from and the value is the filename without the directory and extension.

        :param file_name: The filename without the directory and extension
        :type file_name: str
        :param dirHH: The directory to create the header file in
        :type dirHH: str
        :param **args: A dictionary of directories to include files from and the filenames
        :type **args: dict[str, str]
        :returns: The path to the created header file
        :rtype: str
        """

        include_str = '#pragma once\n'
        madeHeader = dirHH+f'/{file_name}.gen.hh'
        # print(args)
        for dirInclude in args:
            for fileToInclude in args[dirInclude]:
                include_str += f'#include "{dirInclude}/{fileToInclude}.hh"\n'
        open(path+'/../include/'+madeHeader, 'w', encoding='utf-8').write(include_str)
        return f'#include "{madeHeader}"\n'

    def start(self)->None:
        _, self.out = self.decode()
        self.make_decode_tree()
        self.make_enum()
        self.make_bitfields()
        self.make_execute()




if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-D', '--debug', action='store_true', default=False, help='Enable debug mode.')
    args = parser.parse_args()
    text = []
    with open('decoderTree.isa', 'r', encoding="utf-8") as f:
        for line in f:
            line = line.split()
            if line:
                text  = text + line
    gen = Generator(text, args, json_describtion=['implemented.json'])
    gen.gap = gen.tab
    gen.start()
import json
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
            return (' '*(self.gap+self.tab) + instr + '\n')
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
                        mask = '1'*num + f'<<{int(token[0])}'
                    else:
                        # mask = '1' + '0'*int(token[0])
                        mask = '1' + f'<<{int(token[0])}'

                switch_arg = f'bits & 0b{mask}U'
                out_line = ' '*self.gap + f'switch({switch_arg}) {{\n'

                self.gap += self.tab
                case = self.case()
                
                token = self.get_token('decode')
                if token == '}':
                    self.gap -= self.tab
                    if case:
                        out_line += case
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
                        out_line += ' '*self.gap + f'case {token}: {{\n'\
                                    + id\
                                    + ' '*(self.gap+self.tab) + f'break;\n' + ' '*self.gap + '}\n'
                else:
                    if decode[1]:
                        out_line += ' '*self.gap + f'case {token}: {{\n'\
                                    + decode[1]\
                                    + ' '*(self.gap+self.tab) + f'break;\n' + ' '*self.gap + '}\n'
            else:
                self.return_token()
                break
        return out_line
    
    def close(self)->None:
        pass

    def shift_gap(self, sym: str)->None:
        pass

    def place_i(self, instr: str)->None:
        pass

    def dump(self)->None:
        with open('decode_tree_auto.cc', 'w', encoding='utf-8') as f:
            f.write(self.out)
        with open('enum_auto.cc', 'w', encoding='utf-8') as f: 
            enum_str = 'enum Instr {\n'
            for _ in self.enum:
                enum_str += f'  {_},\n'
            enum_str = enum_str[:-2] + '\n};\n'
            f.write(enum_str)

    def start(self)->None:
        _, self.out = self.decode()
        self.dump()


if __name__ == '__main__':
    text = []
    with open('decoder.rv.isa', 'r', encoding="utf-8") as f:
        for line in f:
            line = line.split()
            if line:
                text  = text + line
    gen = Generator(text, json_describtion=['instr_dict_rv_i.json'])
    gen.start()


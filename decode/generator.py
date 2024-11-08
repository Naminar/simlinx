import json
class Generator:
    gap=0
    out=''
    pointer=0
    tab=4

    def __init__(self, text: list, json_describtion: list):
        self.txt = text
        self.data = dict()
        for file in json_describtion:
            self.data.update(json.load(open(file, 'r', encoding="utf-8")))


    def get_token(self)->str:
        token = self.txt[self.pointer] if self.pointer < len(self.txt) else None
        self.pointer += 1
        return token

    def return_token(self)->None:
        if self.pointer > 0:
            self.pointer -= 1

    def ID(self)->None:
        instr = self.get_token()
        # if instr.lower() in self.data:
        self.out += ' '*self.gap + instr + '\n'
    
    def decode(self)->None:        
        token = self.get_token()
        if token[:7] == 'decode(' and token[-1] == ')':
            if self.get_token() == '{':
                token = token[7:-1]
                token.split('-')

                mask = None
                if isinstance(token, list):
                    num = int(token[1]) - int(token[0]) + 1
                    mask = '1'*num + '0'*int(token[0])
                elif isinstance(token, int):
                    mask = '1' + '0'*token

                switch_arg = f'bits & 0b{mask}U'
                self.out += ' '*self.gap + f'switch({switch_arg}) {{\n'

                self.gap += self.tab
                self.case()
                self.gap -= self.tab

                if self.get_token() == '}':
                    self.out += ' '*self.gap + f'}}\n'
                return True
            else:
                raise ValueError
        else:
            self.return_token()


    def case(self)->None:
        while True:
            token = self.get_token()
            if token[-1] == ':':
                token = token[:-1]
                self.out += ' '*self.gap + f'case {token}: {{\n'

                self.gap += self.tab
                if not self.decode():
                    self.ID()
                self.gap -= self.tab
                
                self.out += ' '*(self.gap+self.tab) + f'break;\n' + ' '*self.gap + '}\n'
            else:
                self.return_token()
                break
    
    def close(self)->None:
        pass

    def shift_gap(self, sym: str)->None:
        pass

    def place_i(self, instr: str)->None:
        pass

    def dump(self)->None:
        with open('dump.txt', 'w', encoding='utf-8') as f:
            f.write(self.out)

    def start(self)->None:
        # print(self.txt)
        self.decode()
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


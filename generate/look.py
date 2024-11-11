import json

with open('ExecDecode.isa', 'r', encoding='utf-8') as f:
    data = f.read().replace('\n', '')
    data = json.loads(data)
    print(data)
import re
import yaml

def parse_linux_syscalls():
    with open('linux/syscalls.h', 'r') as f:
        syscalls = f.readlines()

    syscall_start = None
    syscalls_enum = dict()
    with open('linux/unistd.h', 'r') as f:
        for line in f.readlines():
            m = re.search('#define __.+ ([0-9]+)', line)
            if m:
                syscall_start = f'SYSCALL({m.group(1)},'
                continue

            m = re.search('__S.+,(.+?)\)',line)
            if m:
                if not syscall_start:
                    continue
                syscall = m.group(1).strip()
                # a little fuction name preprocessing
                syscall = re.sub('^compat_', '', syscall)
                # syscall = re.sub('^sys_', '', syscall)
                # find syscall declaration and print it out
                definitions = [s for s in syscalls if f'{syscall}(' in s]
                if len(definitions) == 0:
                    # syscall definition not found!
                    syscall_start = None
                    continue
                # print(f'{syscall_start}"{syscall}","{definitions[0].strip()}")')
                syscalls_enum[syscall] = re.findall('[0-9]+',syscall_start)[-1]
                syscall_start = None

    # print(syscalls_enum)
    return(syscalls_enum)

def parse_yaml(file_path):
    with open(file_path, 'r') as file:
        try:
            data = yaml.safe_load(file)
            return data
        except yaml.YAMLError as e:
            print(f"Error parsing YAML file: {e}")
            return None
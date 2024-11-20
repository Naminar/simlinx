import re
with open('syscalls2.h', 'r') as f:
    syscalls = f.readlines()

syscall_start = None
with open('unistd2.h', 'r') as f:
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
            syscall = re.sub('^sys', '', syscall)
            # find syscall declaration and print it out
            definitions = [s for s in syscalls if f'{syscall}(' in s]
            if len(definitions) == 0:
                # syscall definition
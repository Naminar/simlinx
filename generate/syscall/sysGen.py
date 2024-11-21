from jinja2 import Template
from sysParser import parse_linux_syscalls, parse_yaml
from termcolor import colored
import os
import subprocess

template_cc = Template("""
#include "syscall/syscall.gen.hh"

{% for syscall in syscalls %}
// {{ syscall.enum}}
// args [{% for i in range(syscall.args|length) %} a{{ i }} {{ syscall.args[i] }}{{ ", " if not loop.last else "" }}{% endfor %}]
void {{ syscall.name }}(Core& core) {
{{ syscall.do | indent(4) }}
    return; 
}
{% endfor %}
""")

template_hh = Template("""
#pragma once
#include <functional>
#include <stdint.h>
#include <stdio.h>
#include "cpu/core.hh"

using simlinx::Core;

{% for syscall in syscalls %}
void {{ syscall.name }}(Core&);
{% endfor %}

using SyscallFunctionType = std::function<void(Core&)>;
std::array<SyscallFunctionType, {{ syscalls|length }}> syscalls = {
    {% for syscall in syscalls %}
    {{ syscall.name }} {{ ", " if not loop.last else "" }}
    {% endfor %}
};
""")

def runcmd(cmd, verbose = False, *args, **kwargs):

    process = subprocess.Popen(
        cmd,
        stdout = subprocess.PIPE,
        stderr = subprocess.PIPE,
        text = True,
        shell = True
    )
    std_out, std_err = process.communicate()
    if verbose:
        print(std_out.strip(), std_err)
    pass



path = os.getcwd()
if 'simlinx' in path:
    path = path.split('simlinx')[0]
    path += 'simlinx/'
else:
    print(colored('simlinx directory not found', 'red'))
    exit()

if not os.path.exists(path + 'generate/syscall/linux'):
    print(colored('Director was not found, createing simlinx/generate/syscall/linux', 'yellow'))
    os.makedirs(path + 'generate/syscall/linux')

if not os.path.exists(path + 'generate/syscall/linux/syscalls.h'): 
    runcmd(f'cd {path+'generate/syscall/linux'}; wget https://raw.githubusercontent.com/torvalds/linux/refs/heads/master/include/linux/syscalls.h', verbose = True)

if not os.path.exists(path + 'generate/syscall/linux/unistd.h'): 
    runcmd(f'cd {path+'generate/syscall/linux'}; wget https://raw.githubusercontent.com/torvalds/linux/refs/heads/master/include/uapi/asm-generic/unistd.h', verbose=True)

syscalls=parse_yaml(path+'generate/syscall/syscalls.yaml')
# print(syscalls)

sys_enum = parse_linux_syscalls(path+'generate/syscall/')

for syscall in syscalls:
    syscall['enum'] = sys_enum[syscall['name']]

output = template_cc.render(syscalls=syscalls)
print(output)


if not os.path.exists(path + 'src/syscall'):
    print(colored('Director was not found, createing simlinx/src/syscall', 'yellow'))
    os.makedirs(path + 'src/syscall')
with open(path + 'src/syscall/syscall.gen.cc', 'w', encoding='utf-8') as f:
    f.write(output)

if not os.path.exists(path + 'include/syscall'):
    print(colored('Director was not found, createing simlinx/include/syscall', 'yellow'))
    os.makedirs(path + 'include/syscall')
with open(path + 'include/syscall/syscall.gen.hh', 'w', encoding='utf-8') as f:
    f.write(template_hh.render(syscalls=syscalls))

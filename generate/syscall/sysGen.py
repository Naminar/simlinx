from jinja2 import Template
from sysParser import parse_linux_syscalls, parse_yaml
from termcolor import colored
import os

template_cc = Template("""
#include "syscall.gen.hh"

{% for syscall in syscalls %}
// {{ syscall.enum}}
// args [{% for i in range(syscall.args|length) %} a{{ i }} {{ syscall.args[i] }}{{ ", " if not loop.last else "" }}{% endfor %}]
void {{ syscall.name }}(simlinx::Core& core) {
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

{% for syscall in syscalls %}
void {{ syscall.name }}(simlinx::Core&);
{% endfor %}

using SyscallFunctionType = std::function<void(simlinx::Core&)>;
std::array<SyscallFunctionType, {{ syscalls|length }}> syscalls = {
    {% for syscall in syscalls %}
    {{ syscall.name }} {{ ", " if not loop.last else "" }}
    {% endfor %}
};
""")

path = os.getcwd()
if 'simlinx' in path:
    path = path.split('simlinx')[0]
    path += 'simlinx/'
else:
    print(colored('simlinx directory not found', 'red'))
    exit()

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

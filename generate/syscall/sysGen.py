from jinja2 import Template
from sysParser import parse_linux_syscalls, parse_yaml

template_cc = Template("""
{% for syscall in syscalls %}
// {{ syscall.enum}}
// args [{% for i in range(syscall.args|length) %} a{{ i }} {{ syscall.args[i] }}{{ ", " if not loop.last else "" }}{% endfor %}]
void {{ syscall.name }}() {
{{ syscall.do | indent(4) }}
    return void; 
}
{% endfor %}
""")

template_hh = Template("""
#pragma once
#include <functional>

{% for syscall in syscalls %}
void {{ syscall.name }}();
{% endfor %}

using SyscallFunctionType = std::function<void()>;
std::array<SyscallFunctionType, {{ syscalls|length }}> syscalls = {
    {% for syscall in syscalls %}
    {{ syscall.name }} {{ ", " if not loop.last else "" }}
    {% endfor %}
};
""")


syscalls=parse_yaml('syscalls.yaml')
# print(syscalls)

sys_enum = parse_linux_syscalls()

for syscall in syscalls:
    syscall['enum'] = sys_enum[syscall['name']]

output = template_cc.render(syscalls=syscalls)
print(output)

with open('syscall.gen.cc', 'w') as f:
    f.write(output)

with open('syscall.gen.hh', 'w') as f:
    f.write(template_hh.render(syscalls=syscalls))

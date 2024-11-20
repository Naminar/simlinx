from jinja2 import Template
from sysGen import parse_yaml

template = Template("""
{% for syscall in syscalls %}
void {{ syscall.name }}() {
// args [{% for i in range(syscall.args|length) %} a{{ i }} {{ syscall.args[i] }}{{ ", " if not loop.last else "" }}{% endfor %}]
{{ syscall.do | indent(4) }}
    return void; 
}
{% endfor %}
""")

syscalls=parse_yaml('syscalls.yaml')
print(syscalls)

output = template.render(syscalls=syscalls)
print(output)

with open('syscall.gen.cc', 'w') as f:
    f.write(output)
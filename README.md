# simlinx

## Manuals
* [The RISC-V Instruction Set Manual Volume I (Unprivileged Architecture)](https://drive.google.com/file/d/1uviu1nH-tScFfgrovvFCrj7Omv8tFtkp/view)
  * [2017](https://riscv.org/wp-content/uploads/2017/05/riscv-spec-v2.2.pdf)
* [The RISC-V Instruction Set Manual: Volume II (Privileged Architecture)](https://drive.google.com/file/d/17GeetSnT5wW3xNuAHI95-SI1gPGd5sJ_/view)
  * [2017](https://riscv.org/wp-content/uploads/2017/05/riscv-privileged-v1.10.pdf)

## Preparing
To set python's environment:
```
python3 -m venv .venv

# for fish
source .venv/bin/activate.fish
# for bash .venv/bin/activate 
```

To install python's dependents:
```bash
pip install -r requirements.txt
```

Auto-generation
```bash
cd generate
python3 generator.py
python3 syscall/sysGen.py
```

```bash
riscv64-linux-gnu-gcc -nostdlib -march=rv64i -mabi=lp64 --static -Wl,-emain riscv-examples/src/fib.cc
riscv64-linux-gnu-objdump -M no-aliases -M numeric -d
```
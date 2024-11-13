# simlinx

* [The RISC-V Instruction Set Manual Volume I (Unprivileged Architecture)](https://drive.google.com/file/d/1uviu1nH-tScFfgrovvFCrj7Omv8tFtkp/view)
  * [2017](https://riscv.org/wp-content/uploads/2017/05/riscv-spec-v2.2.pdf)
* [The RISC-V Instruction Set Manual: Volume II (Privileged Architecture)](https://drive.google.com/file/d/17GeetSnT5wW3xNuAHI95-SI1gPGd5sJ_/view)
  * [2017](https://riscv.org/wp-content/uploads/2017/05/riscv-privileged-v1.10.pdf)

'''
riscv64-linux-gnu-gcc -nostdlib -march=rv64i -mabi=lp64 —static -Wl,-emain fib.cc
'''
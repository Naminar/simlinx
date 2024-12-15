#include "../../page-table-api/sv39/sv39.hh"

void InitMMU() {
  Sv39PageTableBuilder builder;
  builder.SetRootPPN(0x0);
  builder.FirstLevelPtCnfg(0x10, 0x1, NextLevel);
  builder.SecondLevelPtCnfg(0x10, 0x2, NextLevel);
  builder.ThirdLevelPtCnfg(0x10, 0x10, XO);
  builder.ThirdLevelPtCnfg(0xe1, 0xe1, RW);
  builder.EnableTranslation();
}

int fibonacci_recursive(int n) {
  if (n <= 1)
    return 1;
  return fibonacci_recursive(n - 1) + fibonacci_recursive(n - 2);
}

int main() {
  InitMMU();
  return fibonacci_recursive(7);
}
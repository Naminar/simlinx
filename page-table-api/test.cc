#include "builder.hh"

int main() {
  Sv39PageTableBuilder builder;
  builder.SetRootPPN(0x10000);
  builder.EnableTranslation();
  return 0;
}
unsigned fact(unsigned n) {
  if (n == 1) {
    return 1;
  } else {
    return n * fact(n - 1);
  }
}

int main() { return fact(4); }
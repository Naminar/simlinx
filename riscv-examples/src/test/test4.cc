

int call(int x) { return x + 4; }

int main() {
  int a = 7;
  if (a > 3) {
    return call(a);
  } else {
    return call(3);
  }
}
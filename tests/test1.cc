int biu(int a, int b) {

  int c = 0;
  return c;
}

char foo(float a, int b) {
  return 1;
}

char foo(int a, int b) {
  return 0;
}

typedef int CPPHINT;

int main() {
  int ccc, a = 3;
  a = 3 + 5;
  // CppHint: ?, int, ?
  return a;
}

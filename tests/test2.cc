class Base {
  public:
    virtual ~Base() {}
};

class Float : public Base {};

class Int : public Base {};

Base * Dispatcher(int x) {
  if (x > 0) {
    return new Float();
  }
  else {
    return new Int();
  }
}



int main() {
  int x = 5;
  int y = -5;

  // CppHint: ?, int
  return 0;
}

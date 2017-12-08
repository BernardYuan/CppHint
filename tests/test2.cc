class Base {
  public:
    virtual ~Base() {}
};

class Float : public Base {};

class Int : public Base {};

Base *GetInt(int x) {
  return new Int();
}

Base *GetFloat(float x) {
  return new Float();
}

Base *Dispatcher(int x, int y) {
  if (x + y > 0) {
    return new Float();
  }
  else {
    return new Int();
  }
}

Base *Max(int x, int y) {
  if (x > y) {
    return new Float();
  }
  else {
    return new Int();
  }
}


int main() {
  int x = 5;
  int y = -5;

  // CppHint: ?, int, ?
  return 0;
}

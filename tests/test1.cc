class Base {
public:
  virtual ~Base() {}
};

class Float : public Base {
  public:
  float x;
};

class Int: public Base {
  public:
  int x;
};

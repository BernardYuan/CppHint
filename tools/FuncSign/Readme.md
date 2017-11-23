## Function Signature

A simple tool to get all function signatures from input source code.
### Build
Same as the project, `cmake .. && make `

### Example



``` cplusplus
// test_source.cc

// #include<iostream>

extern int external(int, char);

int decl(char, char);

void fun1() {}
void fun2(float, double) {}

class C1 {
  int getPrivateFunc(float, float) {}
public:
  int getPublicFunc(int, int) {}
};

int main(int argc, char *argv) {
  return 0;
}
```


Apply the tool:
`FuncSign test_source.cc`

Here is the result:
```
int, char, int
char, char, int
void
float, double, void
float, float, int
int, int, int
int, char *, int
```

Basically, one line represents one function with: `arg1 type, arg2 type, ..., return type`. It at least contain `return type`.

If you uncomment the line `include<iostream>`, it will also generates the functions from `iostream`.

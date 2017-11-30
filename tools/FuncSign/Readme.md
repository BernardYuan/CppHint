## Function Signature

A simple tool to get all function signatures from input source code.
### Build
Same as the project, `cmake .. && make `

### Example



``` cplusplus
// test_source.cc

// #include<iostream>

extern int decl(int, char);

int def(int, char);

void fun1(void) {}
void fun1(int) {}

typedef void (FP*) (int, int);
FP fun2(...) {}

typedef int AUT
AUT fun3(AUT) {}

auto f = [](double v) {return v;}

class C1 {
  int getPrivateFunc(float, float) {}
public:
  int getPublicFunc(int, int) {}
};
int k(char (*x)[]) {}
int main(int argc, char *argv[]) {
  return 0;
}
```


Apply the tool:
`FuncSign test_source.cc`

Here is the result:
```
decl, int, char, int
def, int, char, int
fun1, void
fun1, int, void
fun2, int
fun3, AUT, AUT
k, char (*)[], int
main, int, char *[], int
```

Basically, one line represents one function with: `function name, arg1 type, arg2 type, ..., return type`. It at least contains 2 items: `function name` and `return type`.

If you uncomment the line `include<iostream>`, it will also generates the functions from `iostream`.

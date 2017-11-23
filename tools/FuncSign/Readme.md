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
external, int, char, int
decl, char, char, int
fun1, void
fun2, float, double, void
getPrivateFunc, float, float, int
getPublicFunc, int, int, int
main, int, char *, int
```

Basically, one line represents one function with: `function name, arg1 type, arg2 type, ..., return type`. It at least contains 2 items: `function name` and `return type`.

If you uncomment the line `include<iostream>`, it will also generates the functions from `iostream`.

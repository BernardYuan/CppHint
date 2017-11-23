##HintRefactor

A simple refactoring tool which modifies a cpp file by detecting the use specified breakpoint <code>CppHint</code> function call and replace it with supplied function call. In addition, it outputs the runtime type of the object returned from the supplied function call.
### Build
Just use the contained Makefile. Run <code>make</code>.

### Example

<code>cat test.c</code>

~~~
#include<stdio.h>

int main(int argc, char** argv) {
	int a;
	float b;
	
	//The break point
	CppHint(b, a);
	return 0;
}
~~~

Apply the tool:
<code>HintRefactor test.c -s "whatever(a,b)" </code>

<code>cat test.c</code>

~~~
#include<stdio.h>

int main(int argc, char** argv) {
	int a;
	float b;
	
	//The break point
	auto hintReturn = whatever(a,b);
	std::out << std::typeid(hintReturn).name() << std::endl;
	return 0;
}
~~~
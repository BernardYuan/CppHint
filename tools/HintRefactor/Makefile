HintRefactor : HintRefactor.o
	clang HintRefactor.o \
	-o HintRefactor \
	-L/usr/local/opt/llvm/lib \
	-lLLVM \
	-lLLVMCore \
	-lc++ \
	-lclangAST \
	-lclangASTMatchers \
	-lclangAnalysis \
	-lclangBasic \
	-lclangDriver \
	-lclangEdit \
	-lclangFrontend \
	-lclangFrontendTool \
	-lclangLex \
	-lclangParse \
	-lclangSema \
	-lclangRewrite \
	-lclangRewriteFrontend \
	-lclangStaticAnalyzerFrontend \
	-lclangStaticAnalyzerCheckers \
	-lclangStaticAnalyzerCore \
	-lclangSerialization \
	-lclangToolingCore \
	-lclangTooling \
	-lclangFormat \

HintRefactor.o : HintRefactor.cpp
	clang -std=c++11 -c HintRefactor.cpp \
	-I/usr/local/opt/llvm/include \
	-I/usr/local/opt/llvm/include/c++/v1 \

clean:
	rm HintRefactor
	rm *.o
//
// Created by leo on 08/09/17.
//

#ifndef CALLGRAPH_PROFILER_UTILS_H
#define CALLGRAPH_PROFILER_UTILS_H
namespace llvm {
class Module;
class StringRef;
class Constant;
}


#define xstr(s) to_str(s)
#define to_str(s) #s
#define CGPROF(X) CpPHiNT_##X

llvm::Constant* createConstantString(llvm::Module& m, llvm::StringRef str);
#endif //CALLGRAPH_PROFILER_UTILS_H

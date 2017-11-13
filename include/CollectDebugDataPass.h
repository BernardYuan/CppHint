

#ifndef PROFILING_INSTRUMENTATION_PASS_H
#define PROFILING_INSTRUMENTATION_PASS_H

#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/IR/CallSite.h"

namespace cgprofiler {

struct CollectDebugDataPass : public llvm::ModulePass {
  static char ID;

  CollectDebugDataPass() : llvm::ModulePass(ID) {}

  bool runOnModule(llvm::Module& m) override;
};
}

#endif

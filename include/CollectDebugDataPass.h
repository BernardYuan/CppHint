

#ifndef PROFILING_INSTRUMENTATION_PASS_H
#define PROFILING_INSTRUMENTATION_PASS_H

#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/IR/CallSite.h"

namespace CppHinter {

struct CollectDebugDataPass : public llvm::ModulePass {
  static char ID;
  CollectDebugDataPass() : llvm::ModulePass(ID) {}
  bool runOnModule(llvm::Module& m) override;
 private:
  std::string ExtractMetadataType(llvm::Metadata const *type, bool is_scope);
};
}

#endif

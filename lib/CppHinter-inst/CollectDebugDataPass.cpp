#include <iostream>

#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/CallSite.h"
#include "llvm/IR/DebugInfo.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Transforms/Utils/ModuleUtils.h"

#include "CollectDebugDataPass.h"

#include "Utils.h"


using namespace cgprofiler;

namespace cgprofiler {

char CollectDebugDataPass::ID = 0;

}  // namespace cgprofiler

bool CollectDebugDataPass::runOnModule(llvm::Module& module) {
  return true;
}


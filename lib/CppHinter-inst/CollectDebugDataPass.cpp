#include <iostream>

#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/CallSite.h"
#include "llvm/IR/DebugInfo.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Transforms/Utils/ModuleUtils.h"

#include "CollectDebugDataPass.h"

#include "Utils.h"


using namespace CppHinter;

namespace CppHinter {

char CollectDebugDataPass::ID = 0;

}  // namespace CppHinter

bool CollectDebugDataPass::runOnModule(llvm::Module& module) {
  llvm::DebugInfoFinder finder;
  finder.processModule(module);
  for (auto &prog: finder.subprograms()) {
    auto arg_typs = prog->getType()->getTypeArray();
    for (unsigned i= 0; i < arg_typs->getNumOperands(); i++) {
      if (auto operand = arg_typs->getOperand(i).get()) {
        std::cout << ExtractMetadataType(operand, false) << std::endl;
      }
    }
  }
  return true;
}

std::string CollectDebugDataPass::ExtractMetadataType(
    llvm::Metadata const *type, bool is_scope) {
  if (!type) return "";
  if (auto basic_type = llvm::dyn_cast<llvm::DIBasicType>(type)) {
    return basic_type->getName();
  }
  else if (auto derived_type =
      llvm::dyn_cast<llvm::DIDerivedType>(type)) {
    if (derived_type->getTag() == llvm::dwarf::DW_TAG_pointer_type) {
      auto type_string = ExtractMetadataType(
          derived_type->getBaseType().resolve(), false);
      return type_string + "*";
    }
  }
  else if (auto composite_type =
      llvm::dyn_cast<llvm::DICompositeType>(type)) {
    if (composite_type->getTag() == llvm::dwarf::DW_TAG_class_type) {
      auto scope = ExtractMetadataType(
          composite_type->getScope().resolve(), true);
      if (is_scope) {
        return scope + composite_type->getName().str() + "::";
      }
      else {
        return "class." + scope + composite_type->getName().str();
      }
    }
  }
  else if (auto ns = llvm::dyn_cast<llvm::DINamespace>(type)) {
    auto scope = ExtractMetadataType(ns->getScope(), is_scope);
    return scope + ns->getName().str() + "::";
  }
  return "";
}


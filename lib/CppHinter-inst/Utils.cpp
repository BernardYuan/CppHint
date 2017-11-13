//
// Created by leo on 12/11/17.
//

#include "Utils.h"

llvm::Constant* createConstantString(llvm::Module& m,
                                     llvm::StringRef str) {
  auto& context = m.getContext();
  auto* name = llvm::ConstantDataArray::getString(context, str, true);
  auto* int8Ty = llvm::Type::getInt8Ty(context);
  auto* arrayTy = llvm::ArrayType::get(int8Ty, str.size() + 1);
  auto* asStr = new llvm::GlobalVariable(
      m, arrayTy, true, llvm::GlobalValue::PrivateLinkage, name);

  auto* zero = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0);
  llvm::Value* indices[] = {zero, zero};
  return llvm::ConstantExpr::getInBoundsGetElementPtr(arrayTy, asStr, indices);
}


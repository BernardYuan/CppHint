#include <iostream>
#include <vector>
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Parse/ParseAST.h"
#include "clang/Frontend/CompilerInstance.h"

class VariableVisitor : public clang::RecursiveASTVisitor<VariableVisitor> {
 public:
  typedef std::vector<std::pair<std::string, std::string>> VariableStack;

  bool TraverseStmt(clang::Stmt *s) {
    if (auto decl = clang::dyn_cast_or_null<clang::DeclStmt>(s)) {
      for (auto b = decl->decl_begin(), e = decl->decl_end(); b != e; b++) {
        if (auto var = clang::dyn_cast<clang::VarDecl>(*b)) {
          if (var->getName() == "cpp_hint_end") {
            for (auto pair: stack_) {
              std::cout << pair.first << " " << pair.second << std::endl;
            }
            return false;
          }
          stack_.push_back(std::make_pair(var->getType().getAsString(),
                                          var->getName()));
        }
      }
    }
    if (StatmentWithNewScope(s)) {
      auto size = stack_.size();
      bool res = clang::RecursiveASTVisitor<VariableVisitor>::TraverseStmt(s);
      while (size != stack_.size()) {
        auto &back = stack_.back();
        stack_.pop_back();
      }
      return res;
    }
    return true;
  }

 private:
  bool StatmentWithNewScope(clang::Stmt *s) {
    return (s != nullptr) && (
        clang::isa<clang::CompoundStmt>(s) || clang::isa<clang::IfStmt>(s) ||
        clang::isa<clang::WhileStmt>(s) || clang::isa<clang::DoStmt>(s) ||
        clang::isa<clang::SwitchStmt>(s) || clang::isa<clang::ForStmt>(s));
  }


 private:
  VariableStack stack_;
};

class VariableConsumer : public clang::ASTConsumer {
 public:
  virtual bool HandleTopLevelDecl(clang::DeclGroupRef ref) {
    for (auto b = ref.begin(), e = ref.end(); b != e; b++) {
      visitor_.TraverseDecl(*b);
    }
  }
 private:
  VariableVisitor visitor_;
};

int main(int argc, const char **argv) {
  if (argc != 2) {
    llvm::errs() << "Usage: VariableVisitor <filename>\n";
    return 1;
  }

  clang::CompilerInstance instance;
  instance.createDiagnostics();

  auto &lo = instance.getLangOpts();
  lo.CPlusPlus = 1;
  auto target_option = std::make_shared<clang::TargetOptions>();
  target_option->Triple = llvm::sys::getDefaultTargetTriple();
  auto target_info = clang::TargetInfo::CreateTargetInfo(
      instance.getDiagnostics(), target_option);
  instance.setTarget(target_info);

  instance.createFileManager();
  auto &file_manager = instance.getFileManager();
  instance.createSourceManager(file_manager);
  auto &source_manager = instance.getSourceManager();
  instance.createPreprocessor(clang::TU_Module);
  instance.createASTContext();

  const auto *file_in = file_manager.getFile(argv[1]);
  source_manager.setMainFileID(source_manager.createFileID(
      file_in, clang::SourceLocation(), clang::SrcMgr::C_User));
  instance.getDiagnosticClient().BeginSourceFile(
      instance.getLangOpts(), &instance.getPreprocessor());
  VariableConsumer consumer;

  clang::ParseAST(instance.getPreprocessor(), &consumer,
                  instance.getASTContext());

  return 0;
}

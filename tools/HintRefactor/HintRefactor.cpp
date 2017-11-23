#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Tooling/Refactoring.h"
#include "clang/Tooling/Core/Replacement.h"

#include "llvm/Option/OptTable.h"
#include "llvm/Support/CommandLine.h"

#include <map>
#include <string>

using namespace clang::ast_matchers;
using namespace clang::tooling;
using namespace clang;
using namespace llvm;
using namespace std;

class HintRefactorHandler : public MatchFinder::MatchCallback {
public:
    HintRefactorHandler(map<string, Replacements> *replacementMap, string replaceString) : replacementMap(replacementMap), replacementString(replaceString) {}

    virtual void run(const MatchFinder::MatchResult &Result) {
        ASTContext *Context = Result.Context;
        if (const CallExpr *E = Result.Nodes.getNodeAs<CallExpr>("breakpoint")) {
            // get start location and end location of CppHint.
            FullSourceLoc startLocation = Context->getFullLoc(E->getLocStart());
            FullSourceLoc endLocation = Context->getFullLoc(E->getLocEnd());

            if (startLocation.isValid() && endLocation.isValid()) {
                // Compute the offset.
                unsigned offset = E->getLocEnd().getRawEncoding() - E->getLocStart().getRawEncoding();

                Replacement hintReplacement(*(Result.SourceManager),E->getLocStart(), offset+2,
					"auto hintReturn = " + replacementString + ";\nstd::cout << std::typeid(hintReturn).name()<<std::endl;");

                // add all the replacements to the map so that they are effective.
                auto filePath = hintReplacement.getFilePath().str();
                if (replacementMap->find(filePath)==replacementMap->end()) {
                    (*replacementMap)[filePath] = Replacements(hintReplacement);
                } else {
                    if (auto e = (*replacementMap)[filePath].add(hintReplacement)) {
                        return ;
                    }
                }
            }
        }
    }
private:
    map<string, Replacements> *replacementMap;
    string replacementString;
};

static llvm::cl::OptionCategory MyToolCategory("HintRefactor options");
static cl::opt<string> ReplacementStatement(
    "s",
    cl::desc("The statement to replace CppHint."),
    cl::Required,
    cl::cat(MyToolCategory));

int main(int argc, const char **argv) {
    CommonOptionsParser OptionsParser(argc, argv, MyToolCategory);
    RefactoringTool Tool(OptionsParser.getCompilations(),
                   OptionsParser.getSourcePathList());
        
    HintRefactorHandler hintRefactorHandler(&(Tool.getReplacements()), ReplacementStatement);
    MatchFinder breakPointFinder;

    StatementMatcher breakPointMatcher = 
        callExpr(callee(functionDecl(hasName("CppHint")))).bind("breakpoint");
    
    breakPointFinder.addMatcher(breakPointMatcher, &hintRefactorHandler);

    if (int res = Tool.runAndSave(newFrontendActionFactory(&breakPointFinder).get())) {
        return res;
    }
    return 0;
}

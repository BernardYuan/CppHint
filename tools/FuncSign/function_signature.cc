
//////////////////////////////////////////////////////////////////////////////
//
// function_signature.cc
//
// Generate signatures of all functions
//
// Project          : CppHinter
// Author           : Arthur
// Email            : arthur.r.song@gmail.com
//
// Created by Arthur on 2017-11-19.
//
//////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <clang-c/Index.h>

std::string Convert(const CXString& s) {
  std::string result = clang_getCString(s);
  clang_disposeString(s);
  return result;
}

int main(int argc, char *argv[]) {
  // check argument
  if (argc < 2 || !argv[1]) {
    std::cerr << "Lack of argument. Quitting." << std::endl;
    exit(-1);
  }

  CXIndex index = clang_createIndex(0, 0);
  CXTranslationUnit unit = clang_parseTranslationUnit(
      index,
      argv[1],
      nullptr, 0,
      nullptr, 0,
      CXTranslationUnit_None);

  if (unit == nullptr) {
    std::cerr << "Unable to parse translation unit. Quitting." << std::endl;
    exit(-1);
  }

  CXCursor cursor = clang_getTranslationUnitCursor(unit);
  clang_visitChildren(
      cursor,
      [](CXCursor c, CXCursor parent, CXClientData client_data) {
        // if (clang_Location_isFromMainFile(clang_getCursorLocation(cursor)) == 0)
        //   return CXChildVisit_Continue;

        CXCursorKind kind = clang_getCursorKind(c);
        if ((kind == CXCursorKind::CXCursor_FunctionDecl
             || kind == CXCursorKind::CXCursor_CXXMethod
             || kind == CXCursorKind::CXCursor_FunctionTemplate
             || kind == CXCursorKind::CXCursor_Constructor)) {

          auto type = clang_getCursorType(c);
          // auto cursorKind = clang_getCursorKindSpelling(clang_getCursorKind(c));

          // auto function_name = Convert(clang_getCursorSpelling(c));
          auto return_type   = Convert(clang_getTypeSpelling(clang_getResultType(type)));

          int num_args = clang_Cursor_getNumArguments(c);
          for (int i = 0; i < num_args; ++i) {
            // auto arg_cursor = clang_Cursor_getArgument(c, i);
            // auto arg_name = Convert(clang_getCursorSpelling(arg_cursor));
            // if (arg_name.empty()) {
            //  arg_name = "-";
            // }

            auto arg_type = Convert(clang_getTypeSpelling(clang_getArgType(type, i)));
            std::cout << arg_type << " ";
          }
          std::cout << "::-> " << return_type << "\n";
        }


        return CXChildVisit_Recurse;
      },
      nullptr);

  clang_disposeTranslationUnit(unit);
  clang_disposeIndex(index);
}
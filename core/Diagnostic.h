// Ouptut System

#pragma once

#include "clang/Basic/SourceManager.h"
#include "llvm/Support/raw_ostream.h"
#include <string>

class MyDiagnostic {
public:
    static void report(const clang::SourceManager &SM,
                       clang::SourceLocation loc,
                       const std::string &rule,
                       const std::string &message) {

        llvm::errs() << "[" << rule << "] "
                     << message << " at "
                     << loc.printToString(SM)
                     << "\n";
    }
};
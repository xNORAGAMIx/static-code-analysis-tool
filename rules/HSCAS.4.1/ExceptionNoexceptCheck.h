#pragma once

#include "../../framework/Rule.h"
#include "../../core/Diagnostic.h"

#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/Stmt.h"

using namespace clang;
using namespace clang::ast_matchers;

class ExceptionNoexceptCheck : public Rule {
public:
    std::string name() const override {
        return "HSCAS.4.1";
    }

    void registerMatchers(MatchFinder &finder) override;

    void run(const MatchFinder::MatchResult &result) override;

private:
    bool containsThrow(const Stmt *stmt);
};
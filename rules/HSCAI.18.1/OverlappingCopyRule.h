#pragma once

#include "../../framework/Rule.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang;
using namespace clang::ast_matchers;

class OverlappingCopyRule : public Rule {
public:
    void registerMatchers(MatchFinder &finder) override;
    void run(const MatchFinder::MatchResult &result) override;

    std::string name() const override { return "HSCAI.18.1"; }
};
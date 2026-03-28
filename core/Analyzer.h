#pragma once

#include "../framework/Rule.h"
#include <vector>
#include <memory>
#include "clang/ASTMatchers/ASTMatchFinder.h"

class Analyzer {
public:
    Analyzer();

    void registerRules();
    void setupMatchers();

    clang::ast_matchers::MatchFinder& getFinder();

private:
    std::vector<std::unique_ptr<Rule>> rules;
    clang::ast_matchers::MatchFinder finder;
};
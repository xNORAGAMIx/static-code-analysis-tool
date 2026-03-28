// Base rule class

#pragma once

#include "clang/ASTMatchers/ASTMatchFinder.h"
#include <string>

class Rule : public clang::ast_matchers::MatchFinder::MatchCallback {
public:
    virtual ~Rule() = default;

    virtual std::string name() const = 0;

    virtual void registerMatchers(
        clang::ast_matchers::MatchFinder &finder) = 0;
};
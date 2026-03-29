#pragma once

#include "../../framework/Rule.h"
#include "../../core/Diagnostic.h"

class UnusedParameterCheck : public Rule {
public:
    std::string name() const override {
        return "HSCAA.2.2";
    }

    void registerMatchers(clang::ast_matchers::MatchFinder &finder) override;

    void run(const clang::ast_matchers::MatchFinder::MatchResult &result) override;
};
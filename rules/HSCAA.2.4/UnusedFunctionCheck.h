 #pragma once

#include "../../framework/Rule.h"
#include "../../core/Diagnostic.h"

class UnusedFunctionCheck : public Rule {
public:
    std::string name() const override {
        return "HSCAA.2.4";
    }

    void registerMatchers(clang::ast_matchers::MatchFinder &finder) override;

    void run(const clang::ast_matchers::MatchFinder::MatchResult &result) override;
};
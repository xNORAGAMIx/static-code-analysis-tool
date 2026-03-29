#pragma once

#include "../../framework/Rule.h"
#include "../../core/Diagnostic.h"

class MemberInitializationCheck : public Rule {
public:
    std::string name() const override {
        return "HSCAP.1.4";
    }

    void registerMatchers(clang::ast_matchers::MatchFinder &finder) override;

    void run(const clang::ast_matchers::MatchFinder::MatchResult &result) override;
};
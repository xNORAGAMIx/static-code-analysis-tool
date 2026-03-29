#pragma once

#include "../../framework/Rule.h"
#include "../../core/Diagnostic.h"

class DanglingPointerAssignmentCheck : public Rule {
public:
    std::string name() const override {
        return "HSCAG.8.3";
    }

    void registerMatchers(clang::ast_matchers::MatchFinder &finder) override;

    void run(const clang::ast_matchers::MatchFinder::MatchResult &result) override;
};
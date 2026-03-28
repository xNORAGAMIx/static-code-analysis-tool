#include "Analyzer.h"

#include "../rules/HSCAA.1.2/ReturnValueUsedCheck.h"
#include "../rules/HSCAA.2.1/UnusedVariableCheck.h"

Analyzer::Analyzer() {}

void Analyzer::registerRules() {

    rules.push_back(std::make_unique<ReturnValueUsedCheck>());
    rules.push_back(std::make_unique<UnusedVariableCheck>());
}

void Analyzer::setupMatchers() {
    for (auto &rule : rules) {
        rule->registerMatchers(finder);
    }
}

clang::ast_matchers::MatchFinder& Analyzer::getFinder() {
    return finder;
}
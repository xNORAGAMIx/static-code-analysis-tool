#pragma once

#include "Rule.h"
#include <vector>
#include <memory>

class RuleRegistry {
public:
    void addRule(std::unique_ptr<Rule> rule) {
        rules.push_back(std::move(rule));
    }

    std::vector<std::unique_ptr<Rule>>& getRules() {
        return rules;
    }

private:
    std::vector<std::unique_ptr<Rule>> rules;
};
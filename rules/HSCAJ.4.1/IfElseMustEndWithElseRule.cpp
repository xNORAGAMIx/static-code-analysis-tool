#include "IfElseMustEndWithElseRule.h"
#include "../../core/Diagnostic.h"

using namespace clang;
using namespace clang::ast_matchers;

std::string IfElseMustEndWithElseRule::name() const {
    return "HSCAJ.4.1";
}

void IfElseMustEndWithElseRule::registerMatchers(MatchFinder &finder) {
    finder.addMatcher(
        ifStmt(
            hasElse(
                ifStmt().bind("elseIfChain")
            )
        ).bind("topIf"),
        this
    );
}

static bool chainEndsWithElse(const IfStmt *ifStmt) {
    const IfStmt *current = ifStmt;

    while (current) {
        const Stmt *elseStmt = current->getElse();

        if (!elseStmt) {
            return false;
        }

        if (const auto *nextIf = dyn_cast<IfStmt>(elseStmt)) {
            current = nextIf;
        } else {
            return true; // final else exists
        }
    }

    return true;
}

void IfElseMustEndWithElseRule::run(const MatchFinder::MatchResult &result) {
    const auto *ifNode = result.Nodes.getNodeAs<IfStmt>("topIf");
    if (!ifNode) return;

    // Avoid duplicate reports: only check top-most if
    if (ifNode->getElse()) {
        if (const auto *elseIf = dyn_cast<IfStmt>(ifNode->getElse())) {

            if (!chainEndsWithElse(ifNode)) {
                MyDiagnostic::report(
                    *result.SourceManager,
                    ifNode->getIfLoc(),
                    "HSCAJ.4.1",
                    "if-else-if chain must end with a final else statement"
                );
            }
        }
    }
}
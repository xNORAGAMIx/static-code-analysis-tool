#include "NoRawNewDeleteRule.h"
#include "../../core/Diagnostic.h"

using namespace clang;
using namespace clang::ast_matchers;

std::string NoRawNewDeleteRule::name() const {
    return "HSCAV.6.2";
}

void NoRawNewDeleteRule::registerMatchers(MatchFinder &finder) {
    // Match raw new expressions
    finder.addMatcher(
        cxxNewExpr().bind("newExpr"),
        this
    );

    // Match delete expressions
    finder.addMatcher(
        cxxDeleteExpr().bind("deleteExpr"),
        this
    );
}

void NoRawNewDeleteRule::run(const MatchFinder::MatchResult &result) {
    const auto *newExpr = result.Nodes.getNodeAs<CXXNewExpr>("newExpr");
    if (newExpr) {
        // Avoid placement new
        if (newExpr->getNumPlacementArgs() > 0) return;

        MyDiagnostic::report(
            *result.SourceManager,
            newExpr->getExprLoc(),
            name(),
            "avoid raw 'new'; use smart pointers like std::unique_ptr or std::make_unique"
        );
        return;
    }

    const auto *deleteExpr = result.Nodes.getNodeAs<CXXDeleteExpr>("deleteExpr");
    if (deleteExpr) {
        MyDiagnostic::report(
            *result.SourceManager,
            deleteExpr->getExprLoc(),
            name(),
            "avoid raw 'delete'; use RAII and smart pointers"
        );
        return;
    }
}
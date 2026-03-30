#include "NoAdvancedMemoryRule.h"
#include "../../core/Diagnostic.h"

using namespace clang;
using namespace clang::ast_matchers;

std::string NoAdvancedMemoryRule::name() const {
    return "HSCAV.6.3";
}

void NoAdvancedMemoryRule::registerMatchers(MatchFinder &finder) {
    // Placement new
    finder.addMatcher(
        cxxNewExpr(hasAnyPlacementArg(expr())).bind("placementNew"),
        this
    );

    // Direct calls to ::operator new / delete
    finder.addMatcher(
        callExpr(
            callee(functionDecl(hasAnyName("::operator new", "::operator delete")))
        ).bind("opCall"),
        this
    );

    // Custom global operator new/delete definitions
    finder.addMatcher(
        functionDecl(
            isDefinition(),
            hasAnyName("operator new", "operator delete")
        ).bind("customOp"),
        this
    );
}

void NoAdvancedMemoryRule::run(const MatchFinder::MatchResult &result) {

    if (const auto *newExpr = result.Nodes.getNodeAs<CXXNewExpr>("placementNew")) {
        MyDiagnostic::report(
            *result.SourceManager,
            newExpr->getExprLoc(),
            name(),
            "avoid placement new; advanced memory management is not allowed"
        );
        return;
    }

    if (const auto *call = result.Nodes.getNodeAs<CallExpr>("opCall")) {
        MyDiagnostic::report(
            *result.SourceManager,
            call->getExprLoc(),
            name(),
            "avoid direct calls to operator new/delete; use automatic memory management"
        );
        return;
    }

    if (const auto *func = result.Nodes.getNodeAs<FunctionDecl>("customOp")) {
        // Avoid system headers
        if (result.SourceManager->isInSystemHeader(func->getLocation())) return;

        MyDiagnostic::report(
            *result.SourceManager,
            func->getLocation(),
            name(),
            "custom operator new/delete is not allowed"
        );
    }
}
#include "UnusedParameterCheck.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/Decl.h"

using namespace clang;
using namespace clang::ast_matchers;

void UnusedParameterCheck::registerMatchers(MatchFinder &finder) {
    finder.addMatcher(
        parmVarDecl(
            hasParent(functionDecl())
        ).bind("param"),
        this
    );
}

void UnusedParameterCheck::run(const MatchFinder::MatchResult &result) {
    const auto *param =
        result.Nodes.getNodeAs<ParmVarDecl>("param");

    if (!param)
        return;

    // Ignore unnamed parameters
    if (param->getName().empty())
        return;

    // If used → OK
    if (param->isUsed())
        return;

    MyDiagnostic::report(
        *result.SourceManager,
        param->getLocation(),
        name(),
        "Unused function parameter");
}
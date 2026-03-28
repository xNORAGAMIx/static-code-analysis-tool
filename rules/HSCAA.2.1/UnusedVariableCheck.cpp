#include "UnusedVariableCheck.h"

#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/Decl.h"

using namespace clang;
using namespace clang::ast_matchers;

void UnusedVariableCheck::registerMatchers(MatchFinder &finder) {

    finder.addMatcher(
        varDecl(
            hasLocalStorage(),
            unless(isExpansionInSystemHeader())
        ).bind("var"),
        this
    );
}

void UnusedVariableCheck::run(
    const MatchFinder::MatchResult &result) {

    const auto *var =
        result.Nodes.getNodeAs<VarDecl>("var");

    if (!var)
        return;

    if (var->isUsed())
        return;

    MyDiagnostic::report(
        *result.SourceManager,
        var->getLocation(),
        name(),
        "Unused variable"
    );
}
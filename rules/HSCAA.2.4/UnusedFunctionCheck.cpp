#include "UnusedFunctionCheck.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/Decl.h"

using namespace clang;
using namespace clang::ast_matchers;

void UnusedFunctionCheck::registerMatchers(MatchFinder &finder) {
    finder.addMatcher(
        functionDecl(
            isDefinition(),
            isStaticStorageClass(),   // only static functions
            unless(isMain())          // ignore main()
        ).bind("func"),
        this
    );
}

void UnusedFunctionCheck::run(const MatchFinder::MatchResult &result) {
    const auto *func =
        result.Nodes.getNodeAs<FunctionDecl>("func");

    if (!func)
        return;

    // Ignore implicit / compiler-generated
    if (func->isImplicit())
        return;

    if (func->isUsed())
        return;

    MyDiagnostic::report(
        *result.SourceManager,
        func->getLocation(),
        name(),
        "Unused static function");
}
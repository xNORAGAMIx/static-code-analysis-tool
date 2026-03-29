#include "DynamicTypeUsageCheck.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/DeclCXX.h"
#include "clang/AST/ExprCXX.h"

using namespace clang;
using namespace clang::ast_matchers;

void DynamicTypeUsageCheck::registerMatchers(MatchFinder &finder) {

    finder.addMatcher(
        cxxMemberCallExpr(
            callee(cxxMethodDecl().bind("method")),
            anyOf(
                hasAncestor(cxxConstructorDecl().bind("ctor")),
                hasAncestor(cxxDestructorDecl().bind("dtor"))
            )
        ).bind("call"),
        this
    );
}

void DynamicTypeUsageCheck::run(const MatchFinder::MatchResult &result) {

    const auto *method =
        result.Nodes.getNodeAs<CXXMethodDecl>("method");

    if (!method)
        return;

    // Only care about virtual functions
    if (!method->isVirtual())
        return;

    const auto *call =
        result.Nodes.getNodeAs<CXXMemberCallExpr>("call");

    if (!call)
        return;

    MyDiagnostic::report(
        *result.SourceManager,
        call->getExprLoc(),
        name(),
        "Virtual function called inside constructor/destructor (dynamic type not fully valid)"
    );
}
#include "ReturnValueUsedCheck.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/Expr.h"
#include "clang/AST/Stmt.h"

using namespace clang;
using namespace clang::ast_matchers;

void ReturnValueUsedCheck::registerMatchers(MatchFinder &finder) {

    finder.addMatcher(
        callExpr(
            unless(isExpansionInSystemHeader())
        ).bind("call"),
        this
    );
}

void ReturnValueUsedCheck::run(
    const MatchFinder::MatchResult &result) {

    const auto *call =
        result.Nodes.getNodeAs<CallExpr>("call");

    if (!call)
        return;

    if (call->getType()->isVoidType())
        return;

    const Stmt *parent = result.Context->getParents(*call)[0].get<Stmt>();

    if (!parent)
        return;

    // ❗ KEY LOGIC: unused if it's just a standalone statement
    if (isa<Expr>(parent)) {
        const auto *grandParent =
            result.Context->getParents(*parent)[0].get<Stmt>();

        if (isa<CompoundStmt>(grandParent)) {
            MyDiagnostic::report(
                *result.SourceManager,
                call->getExprLoc(),
                name(),
                "Return value is ignored"
            );
        }
    }
}
#include "DanglingPointerAssignmentCheck.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/Expr.h"
#include "clang/AST/Decl.h"

using namespace clang;
using namespace clang::ast_matchers;

void DanglingPointerAssignmentCheck::registerMatchers(MatchFinder &finder) {

    finder.addMatcher(
        binaryOperator(
            isAssignmentOperator(),
            hasRHS(
                unaryOperator(
                    hasOperatorName("&"),
                    hasUnaryOperand(declRefExpr().bind("rhsVar"))
                )
            )
        ).bind("assign"),
        this
    );
}

void DanglingPointerAssignmentCheck::run(const MatchFinder::MatchResult &result) {

    const auto *assign =
        result.Nodes.getNodeAs<BinaryOperator>("assign");

    const auto *rhsVarRef =
        result.Nodes.getNodeAs<DeclRefExpr>("rhsVar");

    if (!assign || !rhsVarRef)
        return;

    const auto *var =
        dyn_cast<VarDecl>(rhsVarRef->getDecl());

    if (!var)
        return;

    // Check if RHS is local variable
    if (!var->hasLocalStorage())
        return;

    // 🚨 Now check LHS
    const Expr *lhs = assign->getLHS()->IgnoreParenImpCasts();

    // Case 1: global variable
    if (const auto *lhsRef = dyn_cast<DeclRefExpr>(lhs)) {
        if (const auto *lhsVar = dyn_cast<VarDecl>(lhsRef->getDecl())) {

            if (!lhsVar->hasLocalStorage()) {
                MyDiagnostic::report(
                    *result.SourceManager,
                    assign->getOperatorLoc(),
                    name(),
                    "Assigning address of local variable to longer-lived object"
                );
            }
        }
    }

    // Case 2: member variable (this->p)
    if (isa<MemberExpr>(lhs)) {
        MyDiagnostic::report(
            *result.SourceManager,
            assign->getOperatorLoc(),
            name(),
            "Assigning address of local variable to class member (dangling pointer risk)"
        );
    }
}
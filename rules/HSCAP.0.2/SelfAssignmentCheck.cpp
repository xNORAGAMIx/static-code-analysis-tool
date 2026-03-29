#include "SelfAssignmentCheck.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/DeclCXX.h"
#include "clang/AST/Stmt.h"

using namespace clang;
using namespace clang::ast_matchers;

void SelfAssignmentCheck::registerMatchers(MatchFinder &finder) {

    finder.addMatcher(
        cxxMethodDecl(
            isDefinition(),
            anyOf(
                isCopyAssignmentOperator(),
                isMoveAssignmentOperator()
            ),
            unless(isImplicit())
        ).bind("op"),
        this
    );
}

void SelfAssignmentCheck::run(const MatchFinder::MatchResult &result) {

    const auto *method =
        result.Nodes.getNodeAs<CXXMethodDecl>("op");

    if (!method)
        return;

    // No parameters? skip
    if (method->getNumParams() != 1)
        return;

    const auto *body = method->getBody();
    if (!body)
        return;

    bool hasSelfCheck = false;

    // Traverse statements manually
    for (const Stmt *stmt : body->children()) {

        if (!stmt)
            continue;

        const auto *ifStmt = dyn_cast<IfStmt>(stmt);
        if (!ifStmt)
            continue;

        const Expr *cond = ifStmt->getCond();

        if (!cond)
            continue;

        // Look for: this == &param
        if (const auto *binOp = dyn_cast<BinaryOperator>(cond)) {

            if (!binOp->isEqualityOp())
                continue;

            const Expr *lhs = binOp->getLHS()->IgnoreParenImpCasts();
            const Expr *rhs = binOp->getRHS()->IgnoreParenImpCasts();

            // Check pattern: this == &param
            if (isa<CXXThisExpr>(lhs)) {

                if (const auto *unary = dyn_cast<UnaryOperator>(rhs)) {
                    if (unary->getOpcode() == UO_AddrOf) {

                        if (const auto *declRef =
                                dyn_cast<DeclRefExpr>(unary->getSubExpr())) {

                            if (declRef->getDecl() == method->getParamDecl(0)) {
                                hasSelfCheck = true;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    if (!hasSelfCheck) {
        MyDiagnostic::report(
            *result.SourceManager,
            method->getLocation(),
            name(),
            "Assignment operator does not handle self-assignment"
        );
    }
}
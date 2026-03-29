#include "OverlappingCopyRule.h"
#include "../../core/Diagnostic.h"

void OverlappingCopyRule::registerMatchers(MatchFinder &finder) {
    finder.addMatcher(
        callExpr(
            callee(functionDecl(hasAnyName(
                "memcpy",
                "memmove",
                "std::copy"
            ))),
            hasArgument(0, expr().bind("dest")),
            hasArgument(1, expr().bind("src"))
        ).bind("call"),
        this
    );
}

static const ValueDecl* getBaseDecl(const Expr *E) {
    E = E->IgnoreImpCasts();

    if (const auto *D = dyn_cast<DeclRefExpr>(E)) {
        return D->getDecl();
    }

    if (const auto *BO = dyn_cast<BinaryOperator>(E)) {
        return getBaseDecl(BO->getLHS());
    }

    if (const auto *UO = dyn_cast<UnaryOperator>(E)) {
        return getBaseDecl(UO->getSubExpr());
    }

    return nullptr;
}

static bool isSameBase(const Expr *lhs, const Expr *rhs) {
    return getBaseDecl(lhs) && getBaseDecl(lhs) == getBaseDecl(rhs);
}

void OverlappingCopyRule::run(const MatchFinder::MatchResult &result) {
    const auto *call = result.Nodes.getNodeAs<CallExpr>("call");
    const auto *dest = result.Nodes.getNodeAs<Expr>("dest");
    const auto *src  = result.Nodes.getNodeAs<Expr>("src");

    if (!call || !dest || !src) return;

    const auto &SM = *result.SourceManager;

    if (SM.isMacroBodyExpansion(call->getExprLoc())) return;

    // Heuristic: same base object → possible overlap
    if (isSameBase(dest, src)) {
        MyDiagnostic::report(
            SM,
            call->getExprLoc(),
            name(),
            "Possible overlapping copy detected (source and destination may overlap)"
        );
    }
}
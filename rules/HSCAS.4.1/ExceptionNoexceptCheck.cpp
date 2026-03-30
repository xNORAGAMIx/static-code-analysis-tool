#include "ExceptionNoexceptCheck.h"

void ExceptionNoexceptCheck::registerMatchers(MatchFinder &finder) {

    finder.addMatcher(
        functionDecl(
            isDefinition(),
            unless(isDeleted()),
            unless(isDefaulted()),
            unless(hasAttr(attr::NoThrow))
        ).bind("func"),
        this
    );
}

bool ExceptionNoexceptCheck::containsThrow(const Stmt *stmt) {
    if (!stmt) return false;

    if (isa<CXXThrowExpr>(stmt))
        return true;

    for (const Stmt *child : stmt->children()) {
        if (child && containsThrow(child))
            return true;
    }

    return false;
}

void ExceptionNoexceptCheck::run(const MatchFinder::MatchResult &result) {

    const auto *func =
        result.Nodes.getNodeAs<FunctionDecl>("func");

    if (!func)
        return;

    const Stmt *body = func->getBody();
    if (!body)
        return;

    // If function throws → valid
    if (containsThrow(body))
        return;
    if (isa<CXXConstructorDecl>(func) || isa<CXXDestructorDecl>(func))
    return;

    MyDiagnostic::report(
        *result.SourceManager,
        func->getLocation(),
        name(),
        "Function does not throw but is not marked noexcept"
    );
}
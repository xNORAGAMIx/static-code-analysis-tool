#include "RefQualifierCheck.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/DeclCXX.h"

using namespace clang;
using namespace clang::ast_matchers;

void RefQualifierCheck::registerMatchers(MatchFinder &finder) {

    finder.addMatcher(
        cxxMethodDecl(
            isDefinition(),
            unless(isImplicit())
        ).bind("method"),
        this
    );
}

void RefQualifierCheck::run(const MatchFinder::MatchResult &result) {

    const auto *method =
        result.Nodes.getNodeAs<CXXMethodDecl>("method");

    if (!method)
        return;

    const auto *parentClass = method->getParent();
    if (!parentClass)
        return;

    QualType returnType = method->getReturnType();

    // Must return reference
    if (!returnType->isReferenceType())
        return;

    QualType pointee = returnType->getPointeeType();

    const auto *returnRecord =
        pointee->getAsCXXRecordDecl();

    if (!returnRecord)
        return;

    // Check if returning same class
    if (returnRecord != parentClass)
        return;

    // 🚨 Check ref-qualifier
    if (method->getRefQualifier() == RQ_None) {
        MyDiagnostic::report(
            *result.SourceManager,
            method->getLocation(),
            name(),
            "Method returning reference to its class should be ref-qualified (& or &&)"
        );
    }
}
#include "ExplicitConstructorCheck.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/DeclCXX.h"

using namespace clang;
using namespace clang::ast_matchers;

void ExplicitConstructorCheck::registerMatchers(MatchFinder &finder) {

    // Constructors
    finder.addMatcher(
        cxxConstructorDecl(
            isDefinition(),
            unless(isExplicit())
        ).bind("ctor"),
        this
    );

    // Conversion operators
    finder.addMatcher(
        cxxConversionDecl(
            unless(isExplicit())
        ).bind("conv"),
        this
    );
}

void ExplicitConstructorCheck::run(const MatchFinder::MatchResult &result) {

    // -------- Constructors --------
    if (const auto *ctor =
        result.Nodes.getNodeAs<CXXConstructorDecl>("ctor")) {

        if (!ctor)
            return;

        // Skip copy/move constructors
        if (ctor->isCopyConstructor() || ctor->isMoveConstructor())
            return;

        // Only 1 parameter
        if (ctor->getNumParams() != 1)
            return;

        MyDiagnostic::report(
            *result.SourceManager,
            ctor->getLocation(),
            name(),
            "Single-argument constructor should be marked explicit"
        );
    }

    // -------- Conversion Operators --------
    if (const auto *conv =
        result.Nodes.getNodeAs<CXXConversionDecl>("conv")) {

        if (!conv)
            return;

        MyDiagnostic::report(
            *result.SourceManager,
            conv->getLocation(),
            name(),
            "Conversion operator should be marked explicit"
        );
    }
}
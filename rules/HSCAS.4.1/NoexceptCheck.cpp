#include "NoexceptCheck.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/DeclCXX.h"

using namespace clang;
using namespace clang::ast_matchers;

void NoexceptCheck::registerMatchers(MatchFinder &finder) {

    // Destructor
    finder.addMatcher(
        cxxDestructorDecl(isDefinition()).bind("dtor"),
        this
    );

    // Move constructor
    finder.addMatcher(
        cxxConstructorDecl(
            isDefinition(),
            isMoveConstructor()
        ).bind("moveCtor"),
        this
    );

    // Move assignment
    finder.addMatcher(
        cxxMethodDecl(
            isDefinition(),
            isMoveAssignmentOperator()
        ).bind("moveAssign"),
        this
    );
}

void NoexceptCheck::run(const MatchFinder::MatchResult &result) {

    // Destructor
    if (const auto *dtor =
        result.Nodes.getNodeAs<CXXDestructorDecl>("dtor")) {

        if (dtor->getExceptionSpecType() != EST_BasicNoexcept) {
            MyDiagnostic::report(
                *result.SourceManager,
                dtor->getLocation(),
                name(),
                "Destructor should be marked noexcept"
            );
        }
    }

    // Move constructor
    if (const auto *ctor =
        result.Nodes.getNodeAs<CXXConstructorDecl>("moveCtor")) {

        if (ctor->getExceptionSpecType() != EST_BasicNoexcept) {
            MyDiagnostic::report(
                *result.SourceManager,
                ctor->getLocation(),
                name(),
                "Move constructor should be marked noexcept"
            );
        }
    }

    // Move assignment
    if (const auto *method =
        result.Nodes.getNodeAs<CXXMethodDecl>("moveAssign")) {

        if (method->getExceptionSpecType() != EST_BasicNoexcept) {
            MyDiagnostic::report(
                *result.SourceManager,
                method->getLocation(),
                name(),
                "Move assignment operator should be marked noexcept"
            );
        }
    }
}
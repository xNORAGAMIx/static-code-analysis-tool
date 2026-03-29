#include "NameHidingCheck.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/DeclCXX.h"

using namespace clang;
using namespace clang::ast_matchers;

void NameHidingCheck::registerMatchers(MatchFinder &finder) {
    finder.addMatcher(
        cxxRecordDecl(
            isDefinition(),
            hasAnyBase(cxxBaseSpecifier())
        ).bind("class"),
        this
    );
}

void NameHidingCheck::run(const MatchFinder::MatchResult &result) {
    const auto *derived =
        result.Nodes.getNodeAs<CXXRecordDecl>("class");

    if (!derived || !derived->hasDefinition())
        return;

    // Iterate over derived class methods
    for (const auto *method : derived->methods()) {

        std::string methodName = method->getNameAsString();

        // Iterate over base classes
        for (const auto &base : derived->bases()) {

            const auto *baseDecl =
                base.getType()->getAsCXXRecordDecl();

            if (!baseDecl)
                continue;

            // Iterate over base methods
            for (const auto *baseMethod : baseDecl->methods()) {

                if (baseMethod->getNameAsString() != methodName)
                    continue;

                // Check if overriding
                if (method->size_overridden_methods() > 0)
                    continue;

                // 🚨 Name hiding detected
                MyDiagnostic::report(
                    *result.SourceManager,
                    method->getLocation(),
                    name(),
                    "Derived class hides base class method '" + methodName + "'"
                );
            }
        }
    }
}
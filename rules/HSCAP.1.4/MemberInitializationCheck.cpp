#include "MemberInitializationCheck.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/DeclCXX.h"
#include "clang/AST/Decl.h"

using namespace clang;
using namespace clang::ast_matchers;

void MemberInitializationCheck::registerMatchers(MatchFinder &finder) {

    finder.addMatcher(
        cxxRecordDecl(isDefinition()).bind("class"),
        this
    );
}

void MemberInitializationCheck::run(const MatchFinder::MatchResult &result) {

    const auto *cls =
        result.Nodes.getNodeAs<CXXRecordDecl>("class");

    if (!cls)
        return;

    // ============================
    // 🔹 PART 1: NON-STATIC MEMBERS
    // ============================

    for (const auto *field : cls->fields()) {

        if (!field || field->getName().empty())
            continue;

        bool isInitialized = false;

        // 1. In-class initializer
        if (field->hasInClassInitializer()) {
            isInitialized = true;
        }

        // 2. Constructor initialization
        for (const auto *ctor : cls->ctors()) {

            for (const auto *init : ctor->inits()) {

                if (init->isMemberInitializer() &&
                    init->getMember() == field) {
                    isInitialized = true;
                    break;
                }
            }

            if (isInitialized)
                break;
        }

        if (!isInitialized) {
            MyDiagnostic::report(
                *result.SourceManager,
                field->getLocation(),
                name(),
                "Non-static data member '" +
                field->getNameAsString() +
                "' is not initialized"
            );
        }
    }

    // ============================
    // 🔹 PART 2: STATIC MEMBERS
    // ============================

    for (const auto *decl : cls->decls()) {

        const auto *var = dyn_cast<VarDecl>(decl);

        if (!var)
            continue;

        if (!var->isStaticDataMember())
            continue;

        // Ignore inline static (C++17)
        if (var->isInline())
            continue;

        // Check if definition exists
        if (!var->hasDefinition()) {

            MyDiagnostic::report(
                *result.SourceManager,
                var->getLocation(),
                name(),
                "Static data member '" +
                var->getNameAsString() +
                "' is declared but not defined"
            );
        }
    }
}
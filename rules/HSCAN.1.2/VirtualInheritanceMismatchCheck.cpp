#include "VirtualInheritanceMismatchCheck.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/DeclCXX.h"

#include <map>
#include <set>

using namespace clang;
using namespace clang::ast_matchers;

void VirtualInheritanceMismatchCheck::registerMatchers(MatchFinder &finder) {

    finder.addMatcher(
        cxxRecordDecl(isDefinition()).bind("class"),
        this
    );
}

// Recursive helper
void collectBases(const CXXRecordDecl *cls,
                  std::map<const CXXRecordDecl*, std::set<bool>> &baseMap) {

    for (const auto &base : cls->bases()) {

        const auto *baseDecl =
            base.getType()->getAsCXXRecordDecl();

        if (!baseDecl)
            continue;

        bool isVirtual = base.isVirtual();

        baseMap[baseDecl].insert(isVirtual);

        // Recurse
        if (baseDecl->hasDefinition()) {
            collectBases(baseDecl->getDefinition(), baseMap);
        }
    }
}

void VirtualInheritanceMismatchCheck::run(const MatchFinder::MatchResult &result) {

    const auto *cls =
        result.Nodes.getNodeAs<CXXRecordDecl>("class");

    if (!cls || !cls->hasDefinition())
        return;

    std::map<const CXXRecordDecl*, std::set<bool>> baseMap;

    collectBases(cls, baseMap);

    for (const auto &[base, modes] : baseMap) {

        if (modes.size() > 1) {

            MyDiagnostic::report(
                *result.SourceManager,
                cls->getLocation(),
                name(),
                "Base class '" + base->getNameAsString() +
                "' is inherited both virtually and non-virtually"
            );
        }
    }
}
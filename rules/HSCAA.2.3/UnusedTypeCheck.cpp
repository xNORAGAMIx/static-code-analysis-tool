#include "UnusedTypeCheck.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/DeclCXX.h"

using namespace clang;
using namespace clang::ast_matchers;

void UnusedTypeCheck::registerMatchers(MatchFinder &finder)
{
    finder.addMatcher(
        cxxRecordDecl(
            isDefinition(),
            hasAncestor(functionDecl()))
            .bind("type"),
        this);
        
}

void UnusedTypeCheck::run(const MatchFinder::MatchResult &result)
{
    const auto *record =
        result.Nodes.getNodeAs<CXXRecordDecl>("type");

    if (!record)
        return;

    // Ignore unnamed structs/classes
    if (record->getName().empty())
        return;

    if (record->isUsed())
        return;

    MyDiagnostic::report(
        *result.SourceManager,
        record->getLocation(),
        name(),
        "Unused local type (class/struct)");
}
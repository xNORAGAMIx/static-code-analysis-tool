#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"

#include "core/Analyzer.h"

using namespace clang::tooling;

static llvm::cl::OptionCategory MyToolCategory("jargon-master options");

int main(int argc, const char **argv) {

    auto ExpectedParser =
        CommonOptionsParser::create(argc, argv, MyToolCategory);

    if (!ExpectedParser) {
        llvm::errs() << "Error creating parser\n";
        return 1;
    }

    CommonOptionsParser &OptionsParser = ExpectedParser.get();

    ClangTool Tool(
        OptionsParser.getCompilations(),
        OptionsParser.getSourcePathList()
    );

    Analyzer analyzer;
    analyzer.registerRules();
    analyzer.setupMatchers();

    return Tool.run(
        newFrontendActionFactory(&analyzer.getFinder()).get()
    );
}
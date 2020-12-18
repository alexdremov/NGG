#include "ASTLoader/ASTLoader.h"
#include "ASTLoader/ASTDumper.h"
#include "AST/ASTNode.h"
#include "Compiler/NGGCompiler.h"
#include "ASTLoader/LoaderParamsParser.h"

int main(const int argc, const char *argv[]) {
    auto params = LoaderCLParams {};
    params.cTor();
    bool result = params.parseArgs(argc, argv);

    if (!result) {
        printf("error: ngg: Error processing command line arguments\n");
        return EXIT_FAILURE;
    }

    FILE *fileIn = fopen(params.inputFileName, "r");
    if (!fileIn) {
        printf("error: ngg: Error opening input file\n");
        return EXIT_FAILURE;
    }

    NGG::ASTNode *allNodes = nullptr;
    auto *head = NGG::ASTLoader::load(fileIn, &allNodes);

    if (head == nullptr) {
        return EXIT_SUCCESS;
    }

    NGG::NGGCompiler compiler {};
    compiler.cTor(head);

    if (params.graph)
        compiler.dumpGraph();

    compiler.compile();

    if (!compiler.isCompileSuccessful()) {
        compiler.dumpCompileErrorStack(params.inputFileRealName);
        printf("error: ngg: Unsuccessful parse\n");
//        compiler.dTor();
        params.dTor();
        return EXIT_FAILURE;
    }

    FILE *generated = fopen(params.outputFileName, "w");
    NGG::ASTDumper::dumpCode(generated, head);

    fclose(generated);
    free(allNodes);
    fclose(fileIn);
    return EXIT_SUCCESS;
}
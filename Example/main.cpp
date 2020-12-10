#include "LexicalAnalysis/LexParser.h"
#include "AST/AST.h"

int main() {
    String content {};
    content.cTor();
    FILE *sourceCode = fopen("testsource.ngg", "rb");
    content.readFromFile(sourceCode);
    fclose(sourceCode);

    auto res = NGG::lexParse(&content);
    NGG::dumpLexemes(res);

    auto ASTParser = NGG::AST {};
    ASTParser.cTor();

    ASTParser.parse(res);
    ASTParser.dumpErrorStack();

    FILE* grDump = fopen("graph.gv", "w");
    ASTParser.dumpTree(grDump);
    fclose(grDump);

    system("dot -Tsvg graph.gv -o code.svg");

    return 0;
}

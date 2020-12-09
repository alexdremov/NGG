#include "LexicalAnalysis/LexParser.h"
#include "AST/AST.h"

int main() {
    String content{}; content.cTor();
    FILE* sourceCode = fopen("testsource.ngg", "rb");
    content.readFromFile(sourceCode);
    fclose(sourceCode);

    auto res = NGG::lexParse(&content);

    NGG::dumpLexemes(res);

    return 0;
}

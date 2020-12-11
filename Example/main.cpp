#include "LexicalAnalysis/LexParser.h"
#include "AST/AST.h"
#include "Compiler/NGGCompiler.h"

int main() {
    NGG::NGGCompiler compiler{}; compiler.cTor();

    compiler.loadFile("testsource.ngg");
    compiler.dumpErrorStack();

    FILE* graph = fopen("graph.gv", "wb");
    compiler.dumpGraph(graph);
    fclose(graph);
    system("dot -Tsvg graph.gv -o code.svg");

    compiler.compile();
    compiler.saveAsmSource("asm.spus");

    compiler.genBytecode("asm.spus", "bytecode.spub");


    return 0;
}

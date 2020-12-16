//
// Created by Александр Дремов on 15.12.2020.
//

#ifndef NGG_ASTLOADER_H
#define NGG_ASTLOADER_H
#include <cstdio>
#include "AST.h"

namespace NGG {
    class ASTLoader {
        static size_t countNodes(ASTNode* head) {
            if (head == nullptr)
                return 0;
            if (head->getLinkKind() == Kind_None)
                return 0;
            return countNodes(head->getLeft()) + countNodes(head->getRight()) + 1;
        }

        static void nodeRecursiveDump(ASTNode* head, FILE* fOut, size_t& num) {
            if (head == nullptr){
                return;
            }
            nodeDump(head, fOut, num);
            nodeRecursiveDump(head->getLeft(), fOut, num);
            nodeRecursiveDump(head->getRight(), fOut, num);
        }

        static void nodeDump(ASTNode* head, FILE* fOut, size_t& num) {
            if (head == nullptr)
                return;
            if (head->getKind() == Kind_None){
                return;
            }

            switch (head->getKind()) {
                case Kind_Linker:
                    fprintf(fOut, "Linker @");
                    break;
                case Kind_FuncDecl:
                    fprintf(fOut, "FuncDecl \"%s\"", head->getLexeme().getString()->begin());
                    break;
                case Kind_Input:
                    fprintf(fOut, "Input @");
                    break;
                case Kind_Identifier:
                    fprintf(fOut, "Identifier \"%s\"", head->getLexeme().getString()->begin());
                    break;
                case Kind_Number:
                    fprintf(fOut, "Number %lg", head->getLexeme().getDouble());
                    break;
                case Kind_ArgumentsList:
                    fprintf(fOut, "ArgDeclList @");
                    break;
                case Kind_CallList:
                    fprintf(fOut, "ArgCallList @");
                    break;
                case Kind_FuncCall:
                    fprintf(fOut, "FuncCall \"%s\"", head->getLexeme().getString()->begin());
                    break;
                case Kind_ReturnStmt:
                    fprintf(fOut, "ReturnStmt @");
                    break;
                case Kind_AssignExpr: // TODO: str value
                    fprintf(fOut, "AssignExpr \"%s\"", head->getLexeme().getString()->begin());
                    break;
                case Kind_VarDef:
                    fprintf(fOut, "VarDef \"%s\"", head->getLexeme().getString()->begin());
                    break;
                case Kind_Print:
                    fprintf(fOut, "Print @");
                    break;
                case Kind_IfStmt:
                    fprintf(fOut, "IfStmt @");
                    break;
                case Kind_WhileStmt:
                    fprintf(fOut, "WhileStmt @");
                    break;
                case Kind_MaOperator:
                case Kind_MaUnOperator:
                case Kind_CmpOperator:
                    fprintf(fOut, "Operator ");
                    switch (head->getLexeme().getType()) {
                        case Lex_Eq:
                            fprintf(fOut, "\"==\"");
                            break;
                        case Lex_Leq:
                            fprintf(fOut, "\"<=\"");
                            break;
                        case Lex_Geq:
                            fprintf(fOut, "\">=\"");
                            break;
                        case Lex_Neq:
                            fprintf(fOut, "\"!=\"");
                            break;
                        case Lex_Gr:
                            fprintf(fOut, "\">\"");
                            break;
                        case Lex_Le:
                            fprintf(fOut, "\"<\"");
                            break;
                        case Lex_Plus:
                            fprintf(fOut, "\"+\"");
                            break;
                        case Lex_Minus:
                            fprintf(fOut, "\"-\"");
                            break;
                        case Lex_Mul:
                            fprintf(fOut, "\"*\"");
                            break;
                        case Lex_Div:
                            fprintf(fOut, "\"/\"");
                            break;
                        case Lex_Pow:
                            fprintf(fOut, "\"^\"");
                            break;
                        default: {
                            fprintf(fOut, "\"@\"");
                            printf("Undefined operator\n");
                        }
                    }
                    break;
                default: {
                    printf("error: Undefined node\n");
                }
            }
            num += 1;
            fprintf(fOut, " %lu %lu\n", num + 1, num + 2);
        }

    public:
        static void dump(ASTNode* head, FILE* outFile) {
            fprintf(outFile, "%zu"
                             "\n", countNodes(head));
            size_t num = 1;
            nodeRecursiveDump(head, outFile, num);
        }
    };
}

#endif //NGG_ASTLOADER_H

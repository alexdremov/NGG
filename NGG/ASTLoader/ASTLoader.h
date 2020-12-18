//
// Created by Александр Дремов on 15.12.2020.
//

#ifndef NGG_ASTLOADER_H
#define NGG_ASTLOADER_H

#include <cstdio>
#include "AST/AST.h"
#include "AST/ASTNode.h"

#define GET_STRING StrContainer::New();\
char* secondQuote = strchr(nodeValue + 1, '"');\
*secondQuote = 0; \
strValue->cTor(nodeValue + 1)

namespace NGG {
    class ASTLoader {
        static size_t countNodes(ASTNode *head) {
            if (head == nullptr)
                return 0;
            if (head->getLinkKind() == Kind_None)
                return 0;
            return countNodes(head->getLeft()) + countNodes(head->getRight()) + 1;
        }

        static void nodeBFSDump(ASTNode *head, FILE *fOut) {
            SwiftyList<ASTNode *> list {};
            list.cTor();
            fprintf(fOut, "None @ 0 0\n");

            list.pushBack(head);
            size_t maxNum = 1;

            while (!list.isEmpty()) {
                ASTNode *node = nullptr;
                list.popFront(&node);

                size_t l = 0, r = 0;

                if (node->getLeft() != nullptr && node->getLeft()->getKind() != Kind_None) {
                    l = ++maxNum;
                    list.pushBack(node->getLeft());
                }
                if (node->getRight() != nullptr && node->getRight()->getKind() != Kind_None) {
                    r = ++maxNum;
                    list.pushBack(node->getRight());
                }

                nodeDump(node, fOut, l, r);
            }
        }

        static void nodeDump(ASTNode *head, FILE *fOut, size_t l, size_t r) {
            if (head == nullptr)
                return;
            if (head->getKind() == Kind_None) {
                return;
            }

            switch (head->getKind()) {
                case Kind_FuncDecl:
                    fprintf(fOut, "FuncDecl \"%s\"",
                            strcmp(head->getLexeme().getString()->begin(), "giveYouUp") == 0 ? "main"
                                                                                             : head->getLexeme().getString()->begin());
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
                case Kind_Linker:
                    fprintf(fOut, "Link @");
                    break;
                case Kind_FuncCall:
                    fprintf(fOut, "FuncCall \"%s\"",
                            strcmp(head->getLexeme().getString()->begin(), "giveYouUp") == 0 ? "main"
                                                                                             : head->getLexeme().getString()->begin());
                    break;
                case Kind_ReturnStmt:
                    fprintf(fOut, "ReturnStmt @");
                    break;
                case Kind_AssignExpr:
                {
                    const char* op = "=";
                    switch (head->getLexeme().getType()) {
                        case Lex_AdAssg:
                            op = "+=";
                            break;
                        case Lex_Assg:
                            op = "=";
                            break;
                        case Lex_MiAssg:
                            op = "-=";
                            break;
                        case Lex_MuAssg:
                            op = "*=";
                            break;
                        case Lex_DiAssg:
                            op = "/=";
                            break;
                        default:{
                            printf("error: unknown assign operator");
                        }
                    }
                    fprintf(fOut, "AssignExpr \"%s\"", op);
                    break;
                }
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
                            printf("error: undefined operator: %s\n", lexemeTypeToString(head->getLexeme().getType()));
                        }
                    }
                    break;
                default: {
                    printf("error: undefined node: %s\n", ASTNodeKindToString(head->getKind()));
                }
            }
            fprintf(fOut, " %zu %zu\n", l, r);
        }

    public:
        static void dump(ASTNode *head, FILE *outFile) {
            fprintf(outFile, "%zu"
                             "\n", countNodes(head));
            nodeBFSDump(head, outFile);
        }

        static ASTNode *load(FILE *inFile, ASTNode **allNodes) {
            size_t numNodes = 0;
            fscanf(inFile, "%zu", &numNodes);

            *allNodes = static_cast<ASTNode *>(calloc(numNodes + 1, sizeof(ASTNode)));

            auto *nodesArr = *allNodes;
            char nodeType[100] = {};
            char nodeValue[100] = {};
            for (size_t i = 0; i < numNodes; i++) {
                size_t l = 0, r = 0;
                fscanf(inFile, "%s%s%zu%zu", nodeType, nodeValue, &l, &r);

                ASTNodeKind kind = Kind_None;
                Lexeme value {};
                value.cTor();
                if (strcmp(nodeType, "Link") == 0) {
                    kind = Kind_Linker;
                } else if (strcmp(nodeType, "FuncDecl") == 0) {
                    kind = Kind_FuncDecl;
                    auto *strValue = GET_STRING;
                    if (strcmp(strValue->begin(), "main") == 0) {
                        strValue->dTor();
                        strValue->cTor("giveYouUp");
                    }
                    value.cTor(Lex_FDecl, strValue);
                } else if (strcmp(nodeType, "Input") == 0) {
                    kind = Kind_Input;
                } else if (strcmp(nodeType, "Identifier") == 0) {
                    kind = Kind_Identifier;
                    auto *strValue = GET_STRING;
                    value.cTor(Lex_Identifier, strValue);
                } else if (strcmp(nodeType, "Number") == 0) {
                    kind = Kind_Number;
                    double num = 0;
                    sscanf(nodeValue, "%lg", &num);
                    value.cTor(Lex_Number, num);
                } else if (strcmp(nodeType, "FuncCall") == 0) {
                    kind = Kind_FuncCall;
                    auto *strValue = GET_STRING;
                    if (strcmp(strValue->begin(), "main") == 0) {
                        strValue->dTor();
                        strValue->cTor("giveYouUp");
                    }
                    value.cTor(Lex_None, strValue);
                } else if (strcmp(nodeType, "ReturnStmt") == 0) {
                    kind = Kind_ReturnStmt;
                } else if (strcmp(nodeType, "None") == 0) {
                    kind = Kind_None;
                } else if (strcmp(nodeType, "AssignExpr") == 0) {
                    kind = Kind_AssignExpr;
                    auto *strValue = GET_STRING;
                    LexemeType type = Lex_Assg;
                    if (strcmp(strValue->begin(), "+=") == 0)
                        type = Lex_AdAssg;
                    else if (strcmp(strValue->begin(), "-=") == 0)
                        type = Lex_MiAssg;
                    else if (strcmp(strValue->begin(), "*=") == 0)
                        type = Lex_MuAssg;
                    else if (strcmp(strValue->begin(), "/=") == 0)
                        type = Lex_DiAssg;
                    value.cTor(type);
                } else if (strcmp(nodeType, "VarDef") == 0) {
                    kind = Kind_VarDef;
                    auto *strValue = GET_STRING;
                    value.cTor(Lex_VDecl, strValue);
                } else if (strcmp(nodeType, "Print") == 0) {
                    kind = Kind_Print;
                } else if (strcmp(nodeType, "IfStmt") == 0) {
                    kind = Kind_IfStmt;
                } else if (strcmp(nodeType, "WhileStmt") == 0) {
                    kind = Kind_WhileStmt;
                } else if (strcmp(nodeType, "Operator") == 0) {
                    auto *strValue = GET_STRING;
                    LexemeType type = Lex_None;
                    if (strcmp(strValue->begin(), "==") == 0) {
                        kind = Kind_CmpOperator;
                        type = Lex_Eq;
                    }if (strcmp(strValue->begin(), "<") == 0) {
                        kind = Kind_CmpOperator;
                        type = Lex_Le;
                    }if (strcmp(strValue->begin(), ">") == 0) {
                        kind = Kind_CmpOperator;
                        type = Lex_Gr;
                    } else if (strcmp(strValue->begin(), "<=") == 0) {
                        kind = Kind_CmpOperator;
                        type = Lex_Leq;
                    } else if (strcmp(strValue->begin(), ">=") == 0) {
                        kind = Kind_CmpOperator;
                        type = Lex_Geq;
                    } else if (strcmp(strValue->begin(), "!=") == 0) {
                        kind = Kind_CmpOperator;
                        type = Lex_Neq;
                    } else if (strcmp(strValue->begin(), "-") == 0) {
                        kind = Kind_MaOperator;
                        if (r == 0)
                            kind = Kind_MaUnOperator;
                        type = Lex_Minus;
                    } else if (strcmp(strValue->begin(), "+") == 0) {
                        kind = Kind_MaOperator;
                        if (r == 0)
                            kind = Kind_MaUnOperator;
                        type = Lex_Plus;
                    } else if (strcmp(strValue->begin(), "*") == 0) {
                        kind = Kind_MaOperator;
                        type = Lex_Mul;
                    } else if (strcmp(strValue->begin(), "/") == 0) {
                        kind = Kind_MaOperator;
                        type = Lex_Div;
                    } else if (strcmp(strValue->begin(), "^") == 0) {
                        kind = Kind_MaOperator;
                        type = Lex_Pow;
                    }
                    value.cTor(type);
                } else {
                    printf("Undefined node!\n");
                }
                nodesArr[i].cTor(kind, value, l == 0 ? nullptr : nodesArr + l, r == 0 ? nullptr : nodesArr + r);
            }
            nodesArr[0].setRight(nullptr);
            nodesArr[0].setLeft(nullptr);

            for (size_t i = 0; i < numNodes; i++) {
                if (nodesArr[i].getKind() == Kind_FuncDecl) {
                    nodesArr[i].getRight()->setLinkKind(Kind_Link_NewScope);
                } else if (nodesArr[i].getKind() == Kind_IfStmt) {
                    if (nodesArr[i].getRight()->getRight() != nullptr)
                            nodesArr[i].getRight()->getRight()->setLinkKind(Kind_Link_NewScope);
                    nodesArr[i].getRight()->getLeft()->setLinkKind(Kind_Link_NewScope);
                } else if (nodesArr[i].getKind() == Kind_WhileStmt) {
                    nodesArr[i].getRight()->setLinkKind(Kind_Link_NewScope);
                }
            }
            return *allNodes + 1;
        }
    };
}

#endif //NGG_ASTLOADER_H

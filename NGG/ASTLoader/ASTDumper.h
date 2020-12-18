//
// Created by Александр Дремов on 17.12.2020.
//

#ifndef NGG_ASTDUMPER_H
#define NGG_ASTDUMPER_H
#include "AST/AST.h"
#include "AST/ASTNode.h"
#include "LexicalAnalysis/LexParser.h"

namespace NGG {
    class ASTDumper {
    public:
        static void dumpCode(FILE* file, ASTNode* tree){
            processFurther(file, tree, 0);
        }
    private:
        static void genMargin(FILE* file, size_t margin, bool valueNeeded=false) {
            for (size_t i = 0; i < margin; i++)
                fprintf(file, "\t");
        }

        static void d_Linker(FILE* file, ASTNode *pNode, size_t margin, bool valueNeeded=false) {
            processFurther(file, pNode->getLeft(), margin, valueNeeded);
            processFurther(file, pNode->getRight(), margin, valueNeeded);
        }

        static void d_FuncDecl(FILE* file, ASTNode *pNode, size_t margin, bool valueNeeded=false) {
            auto alias = LexParser::findAlias(Lex_FDecl);
            auto aliasBStart = LexParser::findAlias(Lex_BStart);
            auto aliasBEnd = LexParser::findAlias(Lex_BEnd);
            auto aliasComma = LexParser::findAlias(Lex_Comma);
            auto aliasLPA = LexParser::findAlias(Lex_LPA);
            auto aliasRPA = LexParser::findAlias(Lex_RPA);

            genMargin(file, margin);
            fprintf(file, "\n\n%s %s %s", alias.alias, strcmp(pNode->getLexeme().getString()->begin(), "main") ==  0 ? "giveYouUp" : pNode->getLexeme().getString()->begin(), aliasLPA.alias);

            ASTNode* argNode = pNode->getLeft();

            while (argNode != nullptr && argNode->getKind() != Kind_None) {
                processFurther(file, argNode->getLeft(), 0, true);
                argNode = argNode->getRight();
                if (argNode != nullptr && argNode->getKind() != Kind_None)
                    fprintf(file, "%s ", aliasComma.alias);
            }

            genMargin(file, margin);
            fprintf(file, "%s %s\n", aliasRPA.alias, aliasBStart.alias);
            processFurther(file, pNode->getRight(), margin + 1);
            genMargin(file, margin);
            fprintf(file, "%s\n", aliasBEnd.alias);
        }

        static void d_Identifier(FILE* file,ASTNode *pNode, size_t margin, bool valueNeeded=false) {
            genMargin(file, margin);
            fprintf(file, "%s", pNode->getLexeme().getString()->begin());
        }

        static void d_Number(FILE* file,ASTNode *pNode, size_t margin, bool valueNeeded=false) {
            genMargin(file, margin);
            fprintf(file, "%lg", pNode->getLexeme().getDouble());
        }

        static void d_AssignExpr(FILE* file,ASTNode *pNode, size_t margin, bool valueNeeded=false) {
            auto alias = LexParser::findAlias(pNode->getLexeme().getType());
            auto aliasStEnd = LexParser::findAlias(Lex_StEnd);
            genMargin(file, margin);
            processFurther(file, pNode->getLeft(), 0);
            fprintf(file, " %s ", alias.alias);
            processFurther(file, pNode->getRight(), 0, true);
            fprintf(file, " %s\n", aliasStEnd.alias);
        }

        static void d_VarDef(FILE* file,ASTNode *pNode, size_t margin, bool valueNeeded=false) {
            auto alias = LexParser::findAlias(pNode->getLexeme().getType());
            auto aliasStEnd = LexParser::findAlias(Lex_StEnd);
            auto aliasAssg = LexParser::findAlias(Lex_Assg);

            genMargin(file, margin);
            fprintf(file, "%s %s ", alias.alias, pNode->getLexeme().getString()->begin());
            if (pNode->getLeft() != nullptr && pNode->getLeft()->getKind() != Kind_None) {
                fprintf(file, "%s ", aliasAssg.alias);
                processFurther(file, pNode->getLeft(), 0, true);
            }
            fprintf(file, " %s\n", aliasStEnd.alias);
        }

        static void d_MaUnOperator(FILE* file,ASTNode *pNode, size_t margin, bool valueNeeded=false) {
            auto aliasLPA = LexParser::findAlias(Lex_LPA);
            auto aliasRPA = LexParser::findAlias(Lex_RPA);

            auto aliasOp = LexParser::findAlias(pNode->getLexeme().getType());
            fprintf(file, " %s", aliasOp.alias);
            if (pNode->getLeft()->getKind() != Kind_Identifier && pNode->getLeft()->getKind() != Kind_Number)
                fprintf(file, "%s", aliasLPA.alias);

            processFurther(file, pNode->getLeft(), 0, true);

            if (pNode->getLeft()->getKind() != Kind_Identifier && pNode->getLeft()->getKind() != Kind_Number)
                fprintf(file, "%s", aliasRPA.alias);
        }

        static void d_MaOperator(FILE* file,ASTNode *pNode, size_t margin, bool valueNeeded=false) {
            auto aliasLPA = LexParser::findAlias(Lex_LPA);
            auto aliasRPA = LexParser::findAlias(Lex_RPA);
            if (pNode->getLeft()->getKind() != Kind_Identifier && pNode->getLeft()->getKind() != Kind_Number)
                fprintf(file, "%s", aliasLPA.alias);
            processFurther(file, pNode->getLeft(), 0, true);
            if (pNode->getLeft()->getKind() != Kind_Identifier && pNode->getLeft()->getKind() != Kind_Number)
                fprintf(file, "%s", aliasRPA.alias);

            auto aliasOp = LexParser::findAlias(pNode->getLexeme().getType());
            fprintf(file, " %s ", aliasOp.alias);

            if (pNode->getRight()->getKind() != Kind_Identifier && pNode->getRight()->getKind() != Kind_Number)
                fprintf(file, "%s", aliasLPA.alias);
            processFurther(file, pNode->getRight(), 0, true);
            if (pNode->getRight()->getKind() != Kind_Identifier && pNode->getRight()->getKind() != Kind_Number)
                fprintf(file, "%s", aliasRPA.alias);
        }

        static void d_FuncCall(FILE* file,ASTNode *pNode, size_t margin, bool valueNeeded=false) {
            auto aliasLPA = LexParser::findAlias(Lex_LPA);
            auto aliasRPA = LexParser::findAlias(Lex_RPA);
            auto aliasComma = LexParser::findAlias(Lex_Comma);
            auto aliasStEnd = LexParser::findAlias(Lex_StEnd);

            genMargin(file, margin);
            fprintf(file, "%s%s", strcmp(pNode->getLexeme().getString()->begin(), "main") ==  0 ? "giveYouUp" : pNode->getLexeme().getString()->begin(), aliasLPA.alias);

            ASTNode* argNode = pNode->getLeft();

            while (argNode != nullptr && argNode->getKind() != Kind_None) {
                processFurther(file, argNode->getLeft(), 0);
                argNode = argNode->getRight();
                if (argNode != nullptr && argNode->getKind() != Kind_None)
                    fprintf(file, "%s ", aliasComma.alias);
            }

            fprintf(file, "%s", aliasRPA.alias);
            if (!valueNeeded)
                fprintf(file, " %s\n", aliasStEnd.alias);
        }

        static void d_CmpOperator(FILE* file,ASTNode *pNode, size_t margin, bool valueNeeded=false) {
            auto aliasLPA = LexParser::findAlias(Lex_LPA);
            auto aliasRPA = LexParser::findAlias(Lex_RPA);
            if (pNode->getLeft()->getKind() != Kind_Identifier && pNode->getLeft()->getKind() != Kind_Number)
                fprintf(file, "%s", aliasLPA.alias);
            processFurther(file, pNode->getLeft(), 0, true);
            if (pNode->getLeft()->getKind() != Kind_Identifier && pNode->getLeft()->getKind() != Kind_Number)
                fprintf(file, "%s", aliasRPA.alias);

            auto aliasOp = LexParser::findAlias(pNode->getLexeme().getType());
            fprintf(file, " %s ", aliasOp.alias);

            if (pNode->getRight()->getKind() != Kind_Identifier && pNode->getRight()->getKind() != Kind_Number)
                fprintf(file, "%s", aliasLPA.alias);
            processFurther(file, pNode->getRight(), 0, true);
            if (pNode->getRight()->getKind() != Kind_Identifier && pNode->getRight()->getKind() != Kind_Number)
                fprintf(file, "%s", aliasRPA.alias);
        }

        static void d_ReturnStmt(FILE* file,ASTNode *pNode, size_t margin, bool valueNeeded=false) {
            auto aliasReturn = LexParser::findAlias(Lex_Return);
            auto aliasStEnd = LexParser::findAlias(Lex_StEnd);
            genMargin(file, margin);
            fprintf(file, "%s ", aliasReturn.alias);
            processFurther(file, pNode->getLeft(), 0, true);
            fprintf(file, " %s\n", aliasStEnd.alias);
        }

        static void d_Print(FILE* file,ASTNode *pNode, size_t margin, bool valueNeeded=false) {
            auto alias = LexParser::findAlias(Lex_Print);
            auto aliasStEnd = LexParser::findAlias(Lex_StEnd);
            genMargin(file, margin);
            fprintf(file, "%s ", alias.alias);
            processFurther(file, pNode->getLeft(), 0, true);
            fprintf(file, " %s\n", aliasStEnd.alias);
        }

        static void d_IfStmt(FILE* file,ASTNode *pNode, size_t margin, bool valueNeeded=false) {
            auto aliasIf = LexParser::findAlias(Lex_If);
            auto aliasLPA = LexParser::findAlias(Lex_LPA);
            auto aliasRPA = LexParser::findAlias(Lex_RPA);
            auto aliasBStart = LexParser::findAlias(Lex_BStart);
            auto aliasBEnd = LexParser::findAlias(Lex_BEnd);
            auto aliasElse = LexParser::findAlias(Lex_Else);

            genMargin(file, margin);
            fprintf(file, "%s %s", aliasIf.alias, aliasLPA.alias);
            processFurther(file, pNode->getLeft(), 0, true);
            fprintf(file, "%s %s\n", aliasRPA.alias, aliasBStart.alias);
            processFurther(file, pNode->getRight()->getLeft(), margin + 1);
            genMargin(file, margin);
            fprintf(file, "%s ", aliasBEnd.alias);

            if (pNode->getRight()->getRight() != nullptr && pNode->getRight()->getRight()->getKind() != Kind_None) {
                fprintf(file, "%s %s\n", aliasElse.alias, aliasBStart.alias);
                processFurther(file, pNode->getRight()->getRight(), margin + 1);
                genMargin(file, margin);
                fprintf(file, "%s ", aliasBEnd.alias);
            }
            fprintf(file, "\n");
        }

        static void d_Input(FILE* file,ASTNode *pNode, size_t margin, bool valueNeeded=false) {
            auto alias = LexParser::findAlias(Lex_Input);
            fprintf(file, " %s ", alias.alias);
        }

        static void d_WhileStmt(FILE* file,ASTNode *pNode, size_t margin, bool valueNeeded=false) {
            auto aliasWhile = LexParser::findAlias(Lex_While);
            auto aliasLPA = LexParser::findAlias(Lex_LPA);
            auto aliasRPA = LexParser::findAlias(Lex_RPA);
            auto aliasBStart = LexParser::findAlias(Lex_BStart);
            auto aliasBEnd = LexParser::findAlias(Lex_BEnd);

            genMargin(file, margin);
            fprintf(file, "%s %s", aliasWhile.alias, aliasLPA.alias);
            processFurther(file, pNode->getLeft(), 0, true);
            fprintf(file, "%s %s\n", aliasRPA.alias, aliasBStart.alias);
            processFurther(file, pNode->getRight(), margin + 1);
            genMargin(file, margin);
            fprintf(file, "%s ", aliasBEnd.alias);

            fprintf(file, "\n");
        }

        static void processFurther(FILE* file, ASTNode *head, size_t margin, bool valueNeeded=false) {
            if (head == nullptr)
                return;
            switch (head->getKind()) {
                case Kind_Linker:
                    d_Linker(file, head, margin, valueNeeded);
                    break;
                case Kind_FuncDecl:
                    d_FuncDecl(file, head, margin, valueNeeded);
                    break;
                case Kind_Identifier:
                    d_Identifier(file, head, margin, valueNeeded);
                    break;
                case Kind_Number:
                    d_Number(file, head, margin, valueNeeded);
                    break;
                case Kind_AssignExpr:
                    d_AssignExpr(file, head, margin, valueNeeded);
                    break;
                case Kind_VarDef:
                    d_VarDef(file, head, margin, valueNeeded);
                    break;
                case Kind_MaUnOperator:
                    d_MaUnOperator(file, head, margin, valueNeeded);
                    break;
                case Kind_MaOperator:
                    d_MaOperator(file, head, margin, valueNeeded);
                    break;
                case Kind_FuncCall:
                    d_FuncCall(file, head, margin, valueNeeded);
                    break;
                case Kind_CmpOperator:
                    d_CmpOperator(file, head, margin, valueNeeded);
                    break;
                case Kind_ReturnStmt:
                    d_ReturnStmt(file, head, margin, valueNeeded);
                    break;
                case Kind_Print:
                    d_Print(file, head, margin, valueNeeded);
                    break;
                case Kind_IfStmt:
                    d_IfStmt(file, head, margin, valueNeeded);
                    break;
                case Kind_Input:
                    d_Input(file, head, margin, valueNeeded);
                    break;
                case Kind_None:
                    break;
                case Kind_WhileStmt:
                    d_WhileStmt(file, head, margin, valueNeeded);
                    break;
                default: {
                    printf("error: undefined node found %s", ASTNodeKindToString(head->getKind()));
                }
            }
        }
    };
}

#endif //NGG_ASTDUMPER_H

//
// Created by Александр Дремов on 09.12.2020.
//

#ifndef NGG_AST_H
#define NGG_AST_H
#include "ASTNode.h"
#include "SwiftyList/SwiftyList.hpp"
#include "LexicalAnalysis/Lexeme.h"
#include "Helpers/Optional.h"
#include "ParsePosition.h"

#define EXPECT_LEXEME(lex, type) (lex.getType() == type)

namespace NGG {
    class AST {
        ASTNode* head;

        static bool expectLex(ParsePosition& pos, LexemeType type){
            Lexeme lex = pos.get();
            return type == lex.getType();
        }

        static bool expectLexAndMove(ParsePosition& pos, LexemeType type){
            Lexeme lex = pos.get();
            bool res = type == lex.getType();
            if (res)
                ++pos;
            return res;
        }

        static ASTNode* getNoneNode(){
            ASTNode* node = ASTNode::New(); node->cTor();
            return node;
        }

        static Lexeme getNoneLexeme(){
            Lexeme lex{}; lex.cTor(Lex_None);
            return lex;
        }

        static Optional<ASTNode*> p_FuncDecl(ParsePosition& pos) {
            Optional<ASTNode*> retValue{}; retValue.cTor();

            if (!expectLexAndMove(pos, Lex_FDecl))
                return retValue;

            if (!expectLex(pos, Lex_Identifier))
                return retValue;

            Lexeme name = pos.getMove();

            if (!expectLexAndMove(pos, Lex_LPA))
                return retValue;

            Optional<ASTNode*> argList = p_ArgumentsList(pos);
            if (!argList.hasValue()){
                argList.cTor(getNoneNode());
            }

            if (!expectLexAndMove(pos, Lex_RPA)) {
                argList.unwrap()->dTor();
                return retValue;
            }

            Optional<ASTNode*> blockStmt = p_BlockStmt(pos);
            if (!blockStmt.hasValue()){
                return retValue;
            }

            ASTNode* node = ASTNode::New();
            node->cTor(Kind_FuncDecl, name, argList.unwrap(), blockStmt.unwrap());

            retValue.cTor(node);
            return retValue;
        }

        static Optional<ASTNode*> p_ArgumentsList(ParsePosition& pos) {
            Optional<ASTNode*> retValue{}; retValue.cTor();

            if (!expectLex(pos, Lex_Identifier))
                return retValue;

            ASTNode* linksHead = ASTNode::New();
            ASTNode* linkEnd = linksHead;

            linksHead->cTor(Kind_Linker, Kind_Identifier);

            Lexeme lex = pos.getMove();

            ASTNode* newNode = ASTNode::New();
            newNode->cTor(Kind_Identifier, lex);
            linkEnd->setLeft(newNode);

            while(!pos.isEnded()) {
                if (!expectLexAndMove(pos, Lex_Comma))
                    break;

                lex = pos.getMove();
                if (lex.getType() != Lex_Identifier)
                    return retValue;

                ASTNode* newLinkNode = ASTNode::New();
                newLinkNode->cTor(Kind_Linker, Kind_Identifier);

                newNode = ASTNode::New();
                newNode->cTor(Kind_Identifier, lex);
                newLinkNode->setLeft(newNode);

                linkEnd->setRight(newLinkNode);
                linkEnd = newLinkNode;
            }

            retValue.cTor(linksHead);
            return retValue;
        }

        static Optional<ASTNode*> p_BlockStmt(ParsePosition& pos) {
            Optional<ASTNode*> retValue{}; retValue.cTor();

            if (!expectLexAndMove(pos, Lex_BStart))
                return retValue;

            ASTNode* linksHead = nullptr;
            ASTNode* linkEnd = nullptr;

            while(!pos.isEnded()) {
                Optional<ASTNode*> statement = p_Statement(pos);
                if (!statement.hasValue())
                    break;

                if (linksHead == nullptr) {
                    linksHead =  ASTNode::New();
                    linksHead->cTor(Kind_Linker, Kind_Statement);
                    linkEnd = linksHead;
                }

                if (linkEnd->getLeft() != nullptr){
                    ASTNode* linksNew = ASTNode::New();
                    linksNew->cTor(Kind_Linker, Kind_Statement);
                    linkEnd = linksNew;
                }
                linkEnd->setLeft(statement.unwrap());
            }

            if (!expectLexAndMove(pos, Lex_BEnd))
                return retValue;

            return retValue;
        }

        static Optional<ASTNode*> p_Statement(ParsePosition& pos) {
            Optional<ASTNode*> retValue{}; retValue.cTor();
            Optional<ASTNode*> content{}; content.cTor();

            content = p_VarDef(pos);
            if (!content.hasValue())
                content = p_Print(pos);
            if (!content.hasValue())
                content = p_PrintLine(pos);
            if (!content.hasValue())
                content = p_AssignExpr(pos);
            if (!content.hasValue())
                content = p_FuncCall(pos);

            if (content.hasValue()){
                if(!expectLexAndMove(pos, Lex_StEnd))
                    return retValue;
            } else {
                content = p_ReturnStmt(pos);
                if (!content.hasValue())
                    content = p_IfStmt(pos);
                if (!content.hasValue())
                    content = p_WhileStmt(pos);
                if (!content.hasValue())
                    content = p_BlockStmt(pos);
                if (!content.hasValue())
                    return retValue;
            }

            return content;
        }

        static Optional<ASTNode*> p_VarDef(ParsePosition& pos) {
            Optional<ASTNode*> retValue{}; retValue.cTor();

            

            return retValue;
        }
    public:
        void cTor() {

        }

        void dTor() {

        }

        static ASTNode* New(){
            auto* ob = static_cast<ASTNode*>(calloc(1, sizeof(ASTNode)));
            ob->cTor();
            return ob;
        }

        static void Delete(ASTNode* ob){
            ob->dTor();
            free(ob);
        }

        void parse(SwiftyList<Lexeme>* input) {

        }

    };
}

#endif //NGG_AST_H

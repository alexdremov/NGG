//
// Created by Александр Дремов on 08.12.2020.
//

#ifndef NGG_ASTNODE_H
#define NGG_ASTNODE_H
#include "ASTNodeKind.h"
#include "LexicalAnalysis/Lexeme.h"
#include "LexicalAnalysis/LexemeType.h"

namespace NGG {
    class ASTNode {
        ASTNode*    left;
        ASTNode*    right;
        ASTNodeKind kind;
        union {
            Lexeme lexeme;
            ASTNodeKind linkKind;
        };
    public:

        void cTor() {
            left   = nullptr;
            right  = nullptr;
            kind   = Kind_None;
            lexeme.cTor();
        }

        void cTor(ASTNodeKind nodeKind, Lexeme lex, ASTNode* nodeLeft = nullptr, ASTNode* nodeRight = nullptr) {
            this->kind = nodeKind;
            this->lexeme = lex;
            this->left = nodeLeft;
            this->right = nodeRight;
        }

        void cTor(ASTNodeKind nodeKind, ASTNodeKind linkKind, ASTNode* nodeLeft = nullptr, ASTNode* nodeRight = nullptr) {
            this->kind = nodeKind;
            this->linkKind = linkKind;
            this->left = nodeLeft;
            this->right = nodeRight;
        }

        void dTor() {
            if (left)
                Delete(left);
            if (right)
                Delete(right);
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

        [[nodiscard]] ASTNode *getLeft() const {
            return left;
        }

        [[nodiscard]] ASTNode *getRight() const {
            return right;
        }

        [[nodiscard]] ASTNodeKind getKind() const {
            return kind;
        }

        [[nodiscard]] const Lexeme &getLexeme() const {
            return lexeme;
        }

        [[nodiscard]] ASTNodeKind getLinkKind() const {
            return linkKind;
        }

        void setLeft(ASTNode *leftNode) {
            this->left = leftNode;
        }

        void setRight(ASTNode *rightNode) {
            this->right = rightNode;
        }
    };
}

#endif //NGG_ASTNODE_H

//
// Created by Александр Дремов on 09.12.2020.
//

#ifndef NGG_LEXEME_H
#define NGG_LEXEME_H
#include <cstdlib>
#include "LexemeType.h"
#include "Helpers/String.h"

namespace NGG {
    class Lexeme {
        LexemeType type;
        bool       stringUsed;
        char*      codeOffset;
        union {
            String sContent;
            double vContent;
        };
    public:
        void cTor(){
            type = Lex_None;
            codeOffset = nullptr;
            stringUsed = false;
        }

        void cTor(LexemeType lexemeType, String content, char* offset= nullptr){
            type = lexemeType;
            sContent = content;
            stringUsed = true;
            codeOffset = offset;
        }

        void cTor(LexemeType lexemeType, char* offset= nullptr){
            type = lexemeType;
            stringUsed = false;
            codeOffset = offset;
        }

        void cTor(LexemeType lexemeType, double content, char* offset= nullptr){
            type = lexemeType;
            vContent = content;
            stringUsed = false;
            codeOffset = offset;
        }

        void dTor(){
            if (stringUsed)
                sContent.dTor();
        }

        static Lexeme* New(){
            auto* ob = static_cast<Lexeme*>(calloc(1, sizeof(Lexeme)));
            ob->cTor();
            return ob;
        }

        static void Delete(Lexeme* ob){
            ob->dTor();
            free(ob);
        }

        [[nodiscard]] char* getCodeOffset() const{
            return codeOffset;
        }

        [[nodiscard]] LexemeType getType() const{
            return type;
        }

        [[nodiscard]] double getDouble() const{
            return vContent;
        }

        [[nodiscard]] String getString() const{
            return sContent;
        }

        [[nodiscard]] bool isStringUsed() const{
            return stringUsed;
        }
    };
}

#endif //NGG_LEXEME_H

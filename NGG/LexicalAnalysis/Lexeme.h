//
// Created by Александр Дремов on 09.12.2020.
//

#ifndef NGG_LEXEME_H
#define NGG_LEXEME_H
#include <cstdlib>
#include "LexemeType.h"
#include "Helpers/StrContainer.h"

namespace NGG {
    class Lexeme {
        LexemeType type;
        bool       stringUsed;
        size_t     col;
        size_t     line;
        char*      codeOffset;
        union {
            StrContainer sContent;
            double vContent;
        };
    public:
        void cTor(){
            type = Lex_None;
            col = 0;
            line = 0;
            stringUsed = false;
        }

        void cTor(LexemeType lexemeType, StrContainer content, char* offset= nullptr){
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

        [[nodiscard]] size_t getLine() const{
            return line;
        }

        [[nodiscard]] size_t getCol() const{
            return col;
        }
        void setLineCol(size_t lexLine, size_t lexCol) {
            line = lexLine;
            col = lexCol;
        }

        [[nodiscard]] LexemeType getType() const{
            return type;
        }

        [[nodiscard]] double getDouble() const{
            return vContent;
        }

        [[nodiscard]] StrContainer getString() const{
            return sContent;
        }

        [[nodiscard]] bool isStringUsed() const{
            return stringUsed;
        }
    };
}

#endif //NGG_LEXEME_H

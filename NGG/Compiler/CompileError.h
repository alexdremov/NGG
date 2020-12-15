//
// Created by Александр Дремов on 12.12.2020.
//

#ifndef NGG_COMPILEERROR_H
#define NGG_COMPILEERROR_H

#include "LexicalAnalysis/Lexeme.h"

namespace NGG {

    struct CompileError {
        StrContainer *msg;
        size_t line;
        size_t col;

        void dTor() {
            if (msg)
                StrContainer::Delete(msg);
        }

        void cTor() {
            msg = nullptr;
            line = 0;
            col = 0;
        }

        void cTor(StrContainer *message, const Lexeme& lex) {
            msg = message;
            line = lex.getLine();
            col = lex.getCol();
        }

        void cTor(const char *message, const Lexeme& lex) {
            msg = StrContainer::New();
            msg->cTor(message);
            line = lex.getLine();
            col = lex.getCol();
        }
    };
}

#endif //NGG_COMPILEERROR_H

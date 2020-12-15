//
// Created by Александр Дремов on 10.12.2020.
//

#ifndef NGG_ASTERROR_H
#define NGG_ASTERROR_H
#include <cstdlib>
#include "LexicalAnalysis/Lexeme.h"

namespace NGG {
    struct ASTError {
        Lexeme      errorIt;
        const char *errorMsg;

        void dTor(){

        }
    };
}

#endif //NGG_ASTERROR_H

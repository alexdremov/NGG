//
// Created by Александр Дремов on 08.12.2020.
//

#ifndef NGG_LEXEMETYPES_H
#define NGG_LEXEMETYPES_H

namespace NGG {
    enum LexemeType {
        #define LEXEME(str, l) Lex_ ## l,
            #include "LexemeType.mpl"
        #undef LEXEME
    };

    static const char* lexemeTypeToString(LexemeType type){
        const char* result = "<none>";
        #define LEXEME(str, l) if (type == Lex_ ## l) return #l;
        #include "LexemeType.mpl"

#undef LEXEME
        return result;
    }
}
#endif //NGG_LEXEMETYPES_H

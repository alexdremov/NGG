//
// Created by Александр Дремов on 09.12.2020.
//

#ifndef NGG_ASTNODEKIND_H
#define NGG_ASTNODEKIND_H
namespace NGG {
    enum ASTNodeKind {
        #define KIND(l) l,
        #include "ASTNodeKind.mpl"
        #undef KIND
    };

    static const char* ASTNodeKindToString(ASTNodeKind type){
        const char* result = "<none>";
        #define KIND(l) if (type == l) return #l;
        #include "ASTNodeKind.mpl"
        #undef KIND
        return result;
    }
}
#endif //NGG_ASTNODEKIND_H

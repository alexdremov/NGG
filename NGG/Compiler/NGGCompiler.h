//
// Created by Александр Дремов on 10.12.2020.
//

#ifndef NGG_NGG_H
#define NGG_NGG_H
#include "ScopeProcessor.h"

namespace NGG {
    class NGGCompiler {
        String         compiledString;
        AST            tree;
        ScopeProcessor env;
    public:
        void cTor() {
            tree.cTor();
            env.cTor();
            compiledString.cTor(static_cast<char*>(calloc(1, sizeof (char))));
        }

        void dTor() {
            tree.dTor();
            env.dTor();
            compiledString.dTor();
        }

        static NGGCompiler *New() {
            auto *ob = static_cast<NGGCompiler *>(calloc(1, sizeof(NGGCompiler)));
            ob->cTor();
            return ob;
        }

        static void Delete(NGGCompiler *ob) {
            ob->dTor();
            free(ob);
        }
    };
}

#endif //NGG_NGG_H

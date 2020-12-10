//
// Created by Александр Дремов on 10.12.2020.
//

#ifndef NGG_SCOPEPROCESSOR_H
#define NGG_SCOPEPROCESSOR_H
#include "IdentifierType.h"
#include "Helpers/String.h"
#include "Helpers/Optional.h"
#include "SwiftyList/SwiftyList.hpp"

namespace NGG {
    class ScopeProcessor {
        struct IdCompiler {
            IdentifierType type;
            String         name;
            union {
                String label;
                size_t memOffset;
            };

            bool operator==(const IdCompiler& other){
                return strcmp(other.name.getStorage(), name.getStorage()) == 0;
            }

            void cTor(const IdentifierType identifierType, const String identifierName){
                type = identifierType;
                name = identifierName;
            }

            void cTor(const IdentifierType identifierType, const String identifierName, String label){
                type = identifierType;
                name = identifierName;
                label = label;
            }

            void cTor(const IdentifierType identifierType, const String identifierName, size_t memOffset){
                type = identifierType;
                name = identifierName;
                memOffset = memOffset;
            }

            void cTor(const String identifierName){
                type = ID_None;
                name = identifierName;
            }
        };
        size_t                 globOffset;
        size_t                 locOffset;
        SwiftyList<IdCompiler> storage;
    public:

        Optional<IdCompiler> get(const String& id) {
            Optional<IdCompiler> retValue{};
            retValue.cTor();
            IdCompiler tmp{}; tmp.cTor(id);
            size_t pos = 0;
            auto result = storage.search(&pos, tmp);
            if (result == LIST_OP_NOTFOUND) {
                return retValue;
            }
            storage.get(pos, &tmp);
            retValue.cTor(tmp);
            return retValue;
        }

        bool def(const String& id, IdentifierType type, String label){
            IdCompiler tmp{}; tmp.cTor(type, id);
            auto trySearch = get(id);
            if (trySearch.hasValue())
                return false;

            storage.pushBack(tmp);
            return true;
        }

        bool def(const String& id, IdentifierType type){
            IdCompiler tmp{}; tmp.cTor(type, id);
            auto trySearch = get(id);
            if (trySearch.hasValue())
                return false;

            storage.pushBack(tmp);
            return true;
        }

        bool deleteLocal() {
            for (size_t i = storage.begin(); i !=0; ) {
                size_t next = storage.nextIterator(i);

                IdCompiler tmp{};
                storage.get(i, &tmp);

                if (tmp.type == ID_Var) {
                    storage.remove(i);
                }

                i = next;
            }
        }

        void cTor() {
            globOffset = 0;
            locOffset  = 0;
            storage = SwiftyList<IdCompiler>(1, 0, nullptr, false);
        }

        void dTor() {

        }

        static ScopeProcessor *New() {
            auto *ob = static_cast<ScopeProcessor *>(calloc(1, sizeof(ScopeProcessor)));
            ob->cTor();
            return ob;
        }

        static void Delete(ScopeProcessor *ob) {
            ob->dTor();
            free(ob);
        }
    };
}

#endif //NGG_SCOPEPROCESSOR_H

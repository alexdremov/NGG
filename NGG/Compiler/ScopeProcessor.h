//
// Created by Александр Дремов on 10.12.2020.
//

#ifndef NGG_SCOPEPROCESSOR_H
#define NGG_SCOPEPROCESSOR_H
#include "IdentifierType.h"
#include "Helpers/StrContainer.h"
#include "Helpers/Stack.h"
#include "Helpers/Optional.h"
#include "SwiftyList/SwiftyList.hpp"

namespace NGG {
    struct IdCompiler {
        IdentifierType type;
        StrContainer   name;
        union {
            StrContainer label;
            size_t memOffset;
        };

        bool operator==(const IdCompiler& other){
            return strcmp(other.name.getStorage(), name.getStorage()) == 0;
        }

        void cTor(const IdentifierType identifierType, const StrContainer identifierName){
            type = identifierType;
            name = identifierName;
        }

        void cTor(const IdentifierType identifierType, const StrContainer identifierName, StrContainer label){
            type = identifierType;
            name = identifierName;
            label = label;
        }

        void cTor(const IdentifierType identifierType, const StrContainer identifierName, size_t memOffset){
            type = identifierType;
            name = identifierName;
            memOffset = memOffset;
        }

        void cTor(const StrContainer identifierName){
            type = ID_None;
            name = identifierName;
        }
    };

    class ScopeProcessor {
        size_t locOffset;
        ClassicStack<SwiftyList<IdCompiler>> storage;

    public:
        Optional<IdCompiler> get(const StrContainer& id) {
            Optional<IdCompiler> retValue{};
            retValue.cTor();
            IdCompiler tmp{}; tmp.cTor(id);
            size_t pos = 0;

            ListOpResult result = LIST_OP_NOTFOUND;
            int i = storage.getSize() - 1;
            for (; i >=0; i--){
                result = storage.get(i).search(&pos, tmp);
                if (result == LIST_OP_OK) {
                    break;
                }
            }
            if (result == LIST_OP_NOTFOUND) {
                return retValue;
            }

            storage.get(i).get(pos, &tmp);
            retValue.cTor(tmp);
            return retValue;
        }

        bool def(const StrContainer& id, IdentifierType type, StrContainer label){
            IdCompiler tmp{}; tmp.cTor(type, id);
            auto trySearch = get(id);
            if (trySearch.hasValue())
                return false;

            tmp.memOffset = getLocalOffset();

            if (type == ID_Var || type == ID_VarGlob) {
                tmp.memOffset = locOffset;
                locOffset++;
            }
            if(storage.getSize() == 1 && type == ID_Var)
                tmp.type = ID_VarGlob;
            tmp.label = label;
            storage.top()->pushBack(tmp);
            return true;
        }

        bool def(const StrContainer& id, IdentifierType type){
            IdCompiler tmp{}; tmp.cTor(type, id);
            auto trySearch = get(id);
            if (trySearch.hasValue())
                return false;

            if (type == ID_Var || type == ID_VarGlob) {
                tmp.memOffset = getLocalOffset();
                locOffset++;
            }
            if(storage.getSize() == 1 && type == ID_Var)
                tmp.type = ID_VarGlob;
            storage.top()->pushBack(tmp);
            return true;
        }

        void deleteLocal() {
            storage.top()->dTor();
            storage.pop();
        }

        void addNewLevel() {
            SwiftyList<IdCompiler> newScope{};
            newScope.cTor(0, 0, nullptr, false);
            storage.push(newScope);
        }

        void cTor() {
            locOffset  = 0;
            storage.cTor(1);
            addNewLevel();
        }

        void dTor() {
            while(!storage.isEmpty()){
                deleteLocal();
            }
            storage.dTor();
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

        [[nodiscard]] size_t getGlobalOffset() const{
            return locOffset;
        }

        [[nodiscard]] size_t getLocalOffset(){
            return storage.top()->getSize();
        }

    };
}

#endif //NGG_SCOPEPROCESSOR_H

//
// Created by Александр Дремов on 10.12.2020.
//

#ifndef NGG_VARTABLE_H
#define NGG_VARTABLE_H

#include "Helpers/StrContainer.h"
#include "Helpers/Stack.h"
#include "Helpers/Optional.h"
#include "SwiftyList/SwiftyList.hpp"

namespace NGG {
    enum VarType {
        Var_Glob,
        Var_Loc,
        Var_None
    };

    struct IdCompiler {
        VarType type;
        StrContainer *name;
        int memOffset;

        bool operator==(const IdCompiler &other) const {
            return strcmp(other.name->getStorage(), name->getStorage()) == 0;
        }

        void cTor(StrContainer *identifierName) {
            name = identifierName;
            type = Var_None;
            memOffset = 0;
        }

        void cTor(StrContainer *identifierName, int varMemOffset) {
            name = identifierName;
            memOffset = varMemOffset;
            type = Var_None;
        }
    };

    class VarTable {
        ClassicStack<SwiftyList<IdCompiler>> storage;
        ClassicStack<size_t> functionScope;
    public:
        Optional<IdCompiler> get(StrContainer *id) {
            Optional<IdCompiler> retValue {};
            retValue.cTor();
            IdCompiler tmp {};
            tmp.cTor(id);
            size_t pos = 0;

            ListOpResult result = LIST_OP_NOTFOUND;
            int i = storage.getSize() - 1;
            int offsetModifier = 0;
            for (; i >= 0; i--) {
                result = storage.get(i).search(&pos, tmp);
                if (i != storage.getSize() - 1)
                    offsetModifier += storage.get(i).getSize();
                if (result == LIST_OP_OK) {
                    break;
                }
            }
            if (result == LIST_OP_NOTFOUND) {
                return retValue;
            }

            storage.get(i).get(pos, &tmp);
            if (tmp.type != Var_Glob)
                tmp.memOffset -= offsetModifier;
            retValue.cTor(tmp);
            return retValue;
        }

        Optional<IdCompiler> getLocalOnly(StrContainer *id) {
            Optional<IdCompiler> retValue {};
            retValue.cTor();
            IdCompiler tmp {};
            tmp.cTor(id);

            size_t pos = 0;
            ListOpResult result = storage.top()->search(&pos, tmp);
            if (result == LIST_OP_NOTFOUND)
                return retValue;
            storage.top()->get(pos, &tmp);
            retValue.cTor(tmp);
            return retValue;
        }

        bool def(StrContainer *id) {
            IdCompiler tmp {};
            tmp.cTor(id);
            auto trySearch = getLocalOnly(id);
            if (trySearch.hasValue())
                return false;

            tmp.memOffset = getLocalOffset();
            tmp.type = Var_Loc;
            if (storage.getSize() == 1)
                tmp.type = Var_Glob;
            storage.top()->pushBack(tmp);
            return true;
        }

        void deleteLocal() {
            auto *list = storage.top();
            list->dTor();
            if (!functionScope.isEmpty()) {
                if (*functionScope.top() == storage.getSize())
                    functionScope.pop();
            }
            storage.pop();
        }

        void addNewLevel(bool function = false) {
            SwiftyList<IdCompiler> newScope {};
            newScope.cTor(0, 0, nullptr, false);
            if (function) {
                functionScope.push(storage.getSize());
            }
            storage.push(newScope);
        }

        void cTor() {
            storage.cTor(1);
            addNewLevel();
        }

        void dTor() {
            while (!storage.isEmpty()) {
                deleteLocal();
            }
            storage.dTor();
        }

        static VarTable *New() {
            auto *ob = static_cast<VarTable *>(calloc(1, sizeof(VarTable)));
            ob->cTor();
            return ob;
        }

        static void Delete(VarTable *ob) {
            ob->dTor();
            free(ob);
        }

        [[nodiscard]] size_t getLocalOffset() {
            return storage.top()->getSize();
        }

        size_t getFunctionOffset() {
            if (functionScope.isEmpty())
                return 0;
            size_t offset = 0;
            for (int i = storage.getSize() - 1; i >= *functionScope.top(); --i)
                offset += storage.get(i).getSize();

            return offset;
        }
    };
}

#endif //NGG_VARTABLE_H

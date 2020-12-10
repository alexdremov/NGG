//
// Created by Александр Дремов on 09.12.2020.
//

#ifndef NGG_PARSEPOSITION_H
#define NGG_PARSEPOSITION_H

#include <cstdlib>
#include "LexicalAnalysis/Lexeme.h"
#include "SwiftyList/SwiftyList.hpp"

namespace NGG {
    class ParsePosition {
        SwiftyList<Lexeme> *input;
        size_t it;
    public:
        void cTor() {
            input = nullptr;
            it = 0;
        }

        void cTor(SwiftyList<Lexeme> *inputList) {
            input = inputList;
            it = inputList->begin();
        }

        void cTor(SwiftyList<Lexeme> *inputList, size_t pos) {
            input = inputList;
            it = pos;
        }

        void dTor() {}

        static ParsePosition *New() {
            auto *ob = static_cast<ParsePosition *>(calloc(1, sizeof(ParsePosition)));
            ob->cTor();
            return ob;
        }

        static void Delete(ParsePosition *ob) {
            ob->dTor();
            free(ob);
        }

        [[nodiscard]] Lexeme get() const {
            Lexeme tmp {};
            tmp.cTor();
            input->get(it, &tmp);
            return tmp;
        }

        [[nodiscard]] size_t getPos() const {
            return it;
        }

        void restore(size_t pos) {
            it = pos;
        }

        [[nodiscard]] Lexeme getMove() {
            Lexeme tmp {};
            tmp.cTor();
            input->get(it, &tmp);
            it = input->nextIterator(it);
            return tmp;
        }

        void operator++() {
            it = input->nextIterator(it);
        }

        void operator--() {
            it = input->prevIterator(it);
        }

        [[nodiscard]] bool isEnded() const{
            return it == 0;
        }
    };
}

#endif //NGG_PARSEPOSITION_H

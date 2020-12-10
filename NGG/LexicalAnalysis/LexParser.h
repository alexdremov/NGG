//
// Created by Александр Дремов on 09.12.2020.
//

#ifndef NGG_LEXPARSER_H
#define NGG_LEXPARSER_H

#include <cstdlib>
#include <cctype>
#include "Helpers/Optional.h"
#include "Lexeme.h"
#include "SwiftyList/SwiftyList.hpp"

#define PARSE_SYMBOL(sym, type) \
    Optional<Lexeme> retVal{}; retVal.cTor();\
    skipInvis(ptr);\
    char* offset = ptr;\
    if (*ptr != sym)\
        return retVal;          \
    Lexeme parsed{}; parsed.cTor(type, offset); \
    ptr++;                    \
    retVal.cTor(parsed);\
    return retVal;

#define PARSE_PHRASE(phr, type) \
    Optional<Lexeme> retVal{}; retVal.cTor();\
    skipInvis(ptr);\
    char* offset = ptr;\
    if (!expectPhrase(ptr, phr))\
        return retVal;\
    \
    Lexeme parsed{}; parsed.cTor(type, offset);\
    retVal.cTor(parsed);\
    return retVal;

namespace NGG {
    class LexParser {
        static bool isCharacter(char ptr) {
            return ('a' <= ptr && ptr <= 'z') || ('A' <= ptr && ptr <= 'Z') || ptr == '_';
        }

        static void getOffsets(const char *offset, const char *start, size_t &line, size_t &col) {
            if (offset == nullptr || start == nullptr)
                return;
            col = 0;
            line = 0;
            while (start <= offset) {
                if (*start == '\n') {
                    line++;
                    col = 0;
                }
                col++;
                start++;
            }
        }

        static bool isDigit(char ptr) {
            return ('0' <= ptr && ptr <= '9');
        }

        static void skipInvis(char *&ptr) {
            while (isspace(*ptr)) ptr++;
        }

        static bool expectPhrase(char *&ptr, const char *cmp) {
            if (ptr == strstr(ptr, cmp)) {
                ptr += strlen(cmp);
                return true;
            }
            return false;
        }

        static Optional<Lexeme> tryParseLex_NumberInt(char *&ptr) {
            Optional<Lexeme> retVal {};
            retVal.cTor();
            skipInvis(ptr);

            int size = 0;
            int number = 0;
            int result = sscanf(ptr, "%d%n", &number, &size);

            if (size == 0 || result <= 0)
                return retVal;

            Lexeme parsed {};

            parsed.cTor(Lex_NumberInt, number, ptr);
            retVal.cTor(parsed);
            ptr += size;
            return retVal;
        };

        static Optional<Lexeme> tryParseLex_Number(char *&ptr) {
            Optional<Lexeme> retVal {};
            retVal.cTor();
            skipInvis(ptr);

            int size = 0;
            double number = 0;
            int result = sscanf(ptr, "%lf%n", &number, &size);

            if (size == 0 || result <= 0)
                return retVal;

            Lexeme parsed {};
            parsed.cTor(Lex_Number, number, ptr);
            retVal.cTor(parsed);
            ptr += size;
            return retVal;
        };

        static Optional<NGG::Lexeme> tryParseLex_Identifier(char *&ptr) {
            Optional<Lexeme> retVal {};
            retVal.cTor();
            skipInvis(ptr);
            char *initPtr = ptr;

            String identifier {};
            identifier.cTor();

            if (!isCharacter(*ptr))
                return retVal;
            identifier.append(*ptr);
            ptr++;

            while (*ptr != '\0') {
                if (isCharacter(*ptr)) {
                    identifier.append(*ptr);
                    ptr++;
                } else {
                    auto number = tryParseLex_NumberInt(ptr);
                    if (!number.hasValue())
                        break;
                    identifier.sEndPrintf(20, "%d", (int) number->getDouble());
                }
            }

            Lexeme parsed {};
            parsed.cTor(Lex_Identifier, identifier, initPtr);
            retVal.cTor(parsed);
            return retVal;
        };

        static Optional<NGG::Lexeme> tryParseLex_FDecl(char *&ptr) {
            PARSE_PHRASE("never gonna", Lex_FDecl)
        };

        static Optional<NGG::Lexeme> tryParseLex_LPA(char *&ptr) {
            PARSE_SYMBOL('(', Lex_LPA)
        };

        static Optional<NGG::Lexeme> tryParseLex_RPA(char *&ptr) {
            PARSE_SYMBOL(')', Lex_RPA)
        };

        static Optional<NGG::Lexeme> tryParseLex_Comma(char *&ptr) {
            PARSE_SYMBOL(',', Lex_Comma)
        };

        static Optional<NGG::Lexeme> tryParseLex_Eq(char *&ptr) {
            PARSE_PHRASE("==", Lex_Eq)
        };

        static Optional<NGG::Lexeme> tryParseLex_Leq(char *&ptr) {
            PARSE_PHRASE("<=", Lex_Leq)
        };

        static Optional<NGG::Lexeme> tryParseLex_Geq(char *&ptr) {
            PARSE_PHRASE(">=", Lex_Geq)
        };

        static Optional<NGG::Lexeme> tryParseLex_Neq(char *&ptr) {
            PARSE_PHRASE("!=", Lex_Neq)
        };

        static Optional<NGG::Lexeme> tryParseLex_Gr(char *&ptr) {
            PARSE_SYMBOL('>', Lex_Gr)
        };

        static Optional<NGG::Lexeme> tryParseLex_Le(char *&ptr) {
            PARSE_SYMBOL('<', Lex_Le)
        };

        static Optional<NGG::Lexeme> tryParseLex_Plus(char *&ptr) {
            PARSE_SYMBOL('+', Lex_Plus)
        };

        static Optional<NGG::Lexeme> tryParseLex_Minus(char *&ptr) {
            PARSE_SYMBOL('-', Lex_Minus)
        };

        static Optional<NGG::Lexeme> tryParseLex_Mul(char *&ptr) {
            PARSE_SYMBOL('*', Lex_Mul)
        };

        static Optional<NGG::Lexeme> tryParseLex_Div(char *&ptr) {
            PARSE_SYMBOL('/', Lex_Div)
        };

        static Optional<NGG::Lexeme> tryParseLex_BStart(char *&ptr) {
            PARSE_PHRASE("strangers", Lex_BStart)
        };

        static Optional<NGG::Lexeme> tryParseLex_BEnd(char *&ptr) {
            PARSE_PHRASE("to love", Lex_BEnd)
        };

        static Optional<NGG::Lexeme> tryParseLex_StEnd(char *&ptr) {
            PARSE_PHRASE("bdum", Lex_StEnd)
        };

        static Optional<NGG::Lexeme> tryParseLex_Return(char *&ptr) {
            PARSE_PHRASE("known each other for so long", Lex_Return)
        };

        static Optional<NGG::Lexeme> tryParseLex_Assg(char *&ptr) {
            PARSE_SYMBOL('=', Lex_Assg)
        };

        static Optional<NGG::Lexeme> tryParseLex_AdAssg(char *&ptr) {
            PARSE_PHRASE("+=", Lex_AdAssg)
        };

        static Optional<NGG::Lexeme> tryParseLex_MiAssg(char *&ptr) {
            PARSE_PHRASE("-=", Lex_MiAssg)
        };

        static Optional<NGG::Lexeme> tryParseLex_MuAssg(char *&ptr) {
            PARSE_PHRASE("*=", Lex_MuAssg)
        };

        static Optional<NGG::Lexeme> tryParseLex_DiAssg(char *&ptr) {
            PARSE_PHRASE("/=", Lex_DiAssg)
        };

        static Optional<NGG::Lexeme> tryParseLex_VDecl(char *&ptr) {
            PARSE_PHRASE("make you", Lex_VDecl)
        };

        static Optional<NGG::Lexeme> tryParseLex_Print(char *&ptr) {
            PARSE_PHRASE("goodbye", Lex_Print)
        };

        static Optional<NGG::Lexeme> tryParseLex_PrintL(char *&ptr) {
            PARSE_PHRASE("desert you", Lex_PrintL)
        };

        static Optional<NGG::Lexeme> tryParseLex_If(char *&ptr) {
            PARSE_PHRASE("you know the rules", Lex_If)
        };

        static Optional<NGG::Lexeme> tryParseLex_Else(char *&ptr) {
            PARSE_PHRASE("and so do i", Lex_Else)
        };

        static Optional<NGG::Lexeme> tryParseLex_While(char *&ptr) {
            PARSE_PHRASE("run around", Lex_While)
        };

        static Optional<NGG::Lexeme> tryParseLex_None(char *&ptr) {
            PARSE_SYMBOL(0, Lex_None)
        };

    public:
        static SwiftyList<NGG::Lexeme> parse(String *content) {
            auto result = SwiftyList<NGG::Lexeme>(0, 0, nullptr, false);
            char *ptr = content->begin();

            #define CHECK_SUCCESS if (parsed.hasValue()){result.pushBack(parsed.unwrap()); continue;}
            #define TRY_PARSE(type) parsed = tryParse ## type(ptr); CHECK_SUCCESS

            for (; ptr < content->end();) {
                Optional<NGG::Lexeme> parsed {};
                parsed.cTor();

                #define LEXEME(l) TRY_PARSE(Lex_ ## l);
                #include "LexemeType.mpl"
                #undef LEXEME

                printf("Undefined sequence at \"%s\"\n", ptr);
            }

            #undef CHECK_SUCCESS
            #undef TRY_PARSE

            char* start = content->begin();
            for(size_t it = result.begin(); it != 0; it = result.nextIterator(it)) {
                size_t col = 0, line = 0;

                Lexeme tmp{}; tmp.cTor();
                result.get(it, &tmp);
                getOffsets(tmp.getCodeOffset(), start, line, col);
                tmp.setLineCol(line, col);
                result.set(it, tmp);
            }

            return result;
        }

        static void dumpLexemes(SwiftyList<NGG::Lexeme> &list, FILE *output = stdout) {
            size_t pos = list.begin();
            for (; pos != 0; list.nextIterator(&pos)) {
                NGG::Lexeme cur {};
                list.get(pos, &cur);

                fprintf(output, "%s", lexemeTypeToString(cur.getType()));
                if (cur.isStringUsed()) {
                    fprintf(output, "<%s>", cur.getString().begin());
                } else {
                    if (cur.getDouble() == 0) {
                        fprintf(output, "<0>");
                    } else {
                        fprintf(output, "<%lg>", cur.getDouble());
                    }

                }
                fprintf(output, " ");
            }
        }
    };
}

#endif //NGG_LEXPARSER_H

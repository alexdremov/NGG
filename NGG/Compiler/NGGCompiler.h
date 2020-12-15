//
// Created by Александр Дремов on 10.12.2020.
//

#ifndef NGG_NGG_H
#define NGG_NGG_H

#include "VarTable.h"
#include "Helpers/ParamsParser.h"
#include "Compiler/CompileError.h"
#include "AST/ASTLoader.h"

namespace NGG {
    class NGGCompiler {
        StrContainer *compiledString;
        SwiftyList<Lexeme> *parsed;
        ClassicStack<CompileError> *cErrors;
        AST tree;
        VarTable env;
        const char *fileName;

        void incRex(size_t num) {
            if (num == 0)
                return;
            else if (num == 1) {
                compiledString->sEndPrintf(
                        "; incrementing rex for 1\n"
                        "inc rex"
                        "; inc complete\n\n",
                        num, num);
            } else {
                compiledString->sEndPrintf(
                        "; incrementing rex for %zu\n"
                        "push %zu\n"
                        "push rex\n"
                        "add\n"
                        "pop rex\n"
                        "; inc complete\n\n",
                        num, num);
            }
        }

        void decRex(size_t num) {
            if (num == 0)
                return;
            else if (num == 1) {
                compiledString->sEndPrintf(
                        "; decrementing rex for 1\n"
                        "dec rex"
                        "; dec complete\n\n",
                        num, num);
            } else {
                compiledString->sEndPrintf(
                        "; decrementing rex for %zu\n"
                        "push rex\n"
                        "push %zu\n"
                        "sub\n"
                        "pop rex\n"
                        "; dec complete\n\n",
                        num, num);
            }
        }

        static StrContainer getJmpLabel(Lexeme lex, const char *specifier = "") {
            StrContainer label {};
            const int randSufLen = 5;
            label.cTor("");

            char randSuffix[randSufLen + 1] = "";
            for (unsigned i = 0; i < randSufLen; i++)
                randSuffix[i] = (char) ('A' + rand() % 24);
            randSuffix[randSufLen] = 0;
            label.sEndPrintf("lbl_%zu_%zu_%s__%s", lex.getLine(), lex.getCol(), specifier, randSuffix);
            return label;
        }

        void varSpecificOperation(const char *operation, IdCompiler var, int incModifier = 0,
                                  size_t line = 0, size_t col = 0) {
            if (var.type == Var_Glob) {
                compiledString->sEndPrintf("%s [%d] ; identifier %s\n",
                                           operation, var.memOffset, var.name->getStorage());
            } else if (var.type == Var_Loc) {
                if (var.memOffset + incModifier >= 0)
                    compiledString->sEndPrintf("%s [rex+%d] ; identifier %s\n",
                                               operation, var.memOffset + incModifier, var.name->getStorage());
                else
                    compiledString->sEndPrintf("%s [rex-%d] ; identifier %s\n",
                                               operation, (-1) * (var.memOffset + incModifier), var.name->getStorage());
            } else {
                CompileError err {};
                err.cTor();
                err.msg = StrContainer::New();
                err.msg->cTor("Can't treat function like a variable: ");
                err.msg->sEndPrintf("%s", var.name->begin());
                err.line = line;
                err.col = col;
                cErrors->push(err);
                return;
            }
        }

        void c_Linker(ASTNode *head, bool valueNeeded, bool noScope = false) {
            bool genNewScope = (head->getLinkKind() == Kind_Link_NewScope) && !noScope;
            if (genNewScope) {
                incRex(env.getLocalOffset());
                env.addNewLevel();
            }
            processFurther(head->getLeft(), valueNeeded);
            if (head->getRight())
                c_Linker(head->getRight(), valueNeeded);

            if (genNewScope) {
                env.deleteLocal();
                decRex(env.getLocalOffset());
            }
        }

        void c_FuncDecl(ASTNode *head) {
            StrContainer *label = head->getLexeme().getString();
            StrContainer *guard = StrContainer::New();
            guard->cTor(label->begin());
            guard->append("_guard");
            compiledString->sEndPrintf("\n; %s function definition (source:%zu:%zu)\n",
                                       label->getStorage(), head->getLexeme().getLine(), head->getLexeme().getCol());

            compiledString->sEndPrintf("jmp %s\n"
                                       "%s:\n", guard->getStorage(),
                                       label->getStorage());

            env.addNewLevel(true);
            if (head->getLeft()->getKind() != Kind_None) {
                ASTNode *cur = head->getLeft();
                while (cur != nullptr) {
                    Lexeme name = cur->getLeft()->getLexeme();
                    env.def(name.getString());
                    cur = cur->getRight();
                }
            }

            processFurther(head->getRight(), false, true);
            compiledString->sEndPrintf("ret\n"
                                       "%s:\n\n", guard->getStorage());
            env.deleteLocal();
        }

        void c_Identifier(ASTNode *head) {
            StrContainer *label = head->getLexeme().getString();

            Optional<IdCompiler> found = env.get(label);
            if (!found.hasValue()) {
                CompileError err {};
                err.cTor("Identifier is not defined: ", head->getLexeme());
                err.msg->sEndPrintf("%s", label->begin());
                cErrors->push(err);
                return;
            }

            varSpecificOperation("push", found.unwrap(), 0, head->getLexeme().getLine(), head->getLexeme().getCol());
        }

        void c_Number(ASTNode *head) {
            compiledString->sEndPrintf("push %lg\n", head->getLexeme().getDouble());
        }

        void c_MaOperator(ASTNode *head) {
            auto type = head->getLexeme().getType();
            processFurther(head->getLeft(), true);
            processFurther(head->getRight(), true);

            switch (type) {
                case Lex_Plus:
                    compiledString->sEndPrintf("add ; %zu:%zu\n",
                                               head->getLexeme().getLine(), head->getLexeme().getCol());
                    break;
                case Lex_Minus:
                    compiledString->sEndPrintf("sub ; %zu:%zu\n",
                                               head->getLexeme().getLine(), head->getLexeme().getCol());
                    break;
                case Lex_Mul:
                    compiledString->sEndPrintf("mul ; %zu:%zu\n",
                                               head->getLexeme().getLine(), head->getLexeme().getCol());
                    break;
                case Lex_Div:
                    compiledString->sEndPrintf("div ; %zu:%zu\n",
                                               head->getLexeme().getLine(), head->getLexeme().getCol());
                    break;
                case Lex_Pow:
                    compiledString->sEndPrintf("pow ; %zu:%zu\n",
                                               head->getLexeme().getLine(), head->getLexeme().getCol());
                    break;
                default: {
                    CompileError err {};
                    err.cTor("Unknown operation in c_MaOperator: ", head->getLexeme());
                    err.msg->sEndPrintf("%s", lexemeTypeToString(type));
                    cErrors->push(err);
                    return;
                }
            }
        }

        void c_AssignExpr(ASTNode *head) {
            auto type = head->getLexeme().getType();

            ASTNode *idNode = head->getRight();
            ASTNode *valNode = head->getLeft();

            StrContainer *name = idNode->getLexeme().getString();
            Optional<IdCompiler> found = env.get(name);

            if (!found.hasValue()) {
                CompileError err {};
                err.cTor("Identifier was not declared", head->getLexeme());
                err.msg->sEndPrintf("%s", head->getLexeme().getString()->begin());
                cErrors->push(err);
                return;
            }

            switch (type) {
                case Lex_AdAssg:
                    processFurther(valNode, true);
                    varSpecificOperation("push", found.unwrap(), 0, head->getLexeme().getLine(),
                                         head->getLexeme().getCol());
                    compiledString->sEndPrintf("add ; Lex_AdAssg %zu:%zu\n",
                                               head->getLexeme().getLine(), head->getLexeme().getCol());
                    break;
                case Lex_MiAssg:
                    varSpecificOperation("push", found.unwrap(), 0, head->getLexeme().getLine(),
                                         head->getLexeme().getCol());
                    processFurther(valNode, true);
                    compiledString->sEndPrintf("sub ; Lex_AdAssg %zu:%zu\n",
                                               head->getLexeme().getLine(), head->getLexeme().getCol());
                    break;
                case Lex_MuAssg:
                    processFurther(valNode, true);
                    varSpecificOperation("push", found.unwrap(), 0, head->getLexeme().getLine(),
                                         head->getLexeme().getCol());
                    compiledString->sEndPrintf("mul ; Lex_AdAssg %zu:%zu\n",
                                               head->getLexeme().getLine(), head->getLexeme().getCol());
                    break;
                case Lex_DiAssg:
                    processFurther(valNode, true);
                    varSpecificOperation("push", found.unwrap(), 0,
                                         head->getLexeme().getLine(), head->getLexeme().getCol());
                    compiledString->sEndPrintf("div ; Lex_AdAssg %zu:%zu\n",
                                               head->getLexeme().getLine(), head->getLexeme().getCol());
                    break;
                case Lex_Assg: {
                    processFurther(head->getRight(), true);
                }
                default: {
                    CompileError err {};
                    err.cTor("Unknown operation in c_AssignExpr: ", head->getLexeme());
                    err.msg->sEndPrintf("%s", lexemeTypeToString(type));
                    cErrors->push(err);
                    return;
                }
            }
            varSpecificOperation("pop", found.unwrap(), 0, head->getLexeme().getLine(), head->getLexeme().getCol());
        }

        void c_VarDef(ASTNode *head) {
            auto type = head->getLexeme();
            bool res = env.def(type.getString());
            if (!res) {
                CompileError err {};
                err.cTor("Can't redeclare variable: ", head->getLexeme());
                err.msg->sEndPrintf("%s", head->getLexeme().getString()->begin());
                cErrors->push(err);
                return;
            }
            processFurther(head->getLeft(), true);
            Optional<IdCompiler> found = env.get(type.getString());

            varSpecificOperation("pop", found.unwrap(), 0,
                                 head->getLexeme().getLine(), head->getLexeme().getCol());
        }

        void c_MaUnOperator(ASTNode *head) {
            auto type = head->getLexeme().getType();
            processFurther(head->getLeft(), true);

            if (type == Lex_Minus) {
                compiledString->sEndPrintf("push -1\n"
                                           "mul ; %zu:%zu\n",
                                           head->getLexeme().getLine(), head->getLexeme().getCol());
            }
        }

        void c_Statement(ASTNode *head) {
            processFurther(head->getLeft());
            processFurther(head->getRight());
        }

        void c_FuncCall(ASTNode *head, bool valueNeeded = false) {
            ASTNode *lastNode = head->getLeft();

            if (lastNode->getKind() != Kind_None) {
                int argOffset = 0;
                while (lastNode != nullptr) {
                    processFurther(lastNode->getLeft(), true);
                    compiledString->sEndPrintf("pop [rex+%zu] ; preparing arg for func call\n",
                                               env.getLocalOffset() + argOffset);
                    lastNode = lastNode->getRight();
                    argOffset++;
                }
            }
            incRex(env.getLocalOffset());
            compiledString->sEndPrintf("call %s\n", head->getLexeme().getString()->begin());
            decRex(env.getLocalOffset());
            if (valueNeeded)
                compiledString->sEndPrintf("push rax ; pushing return value\n");
        }

        void c_CmpOperator(ASTNode *head) {
            auto type = head->getLexeme().getType();

            processFurther(head->getLeft(), true);
            processFurther(head->getRight(), true);
            switch (type) {
                case Lex_Eq:
                    compiledString->sEndPrintf("eq ; ==\n");
                    break;
                case Lex_Leq:
                    compiledString->sEndPrintf("leq ; <=\n");
                    break;
                case Lex_Geq:
                    compiledString->sEndPrintf("geq ; >=\n");
                    break;
                case Lex_Neq:
                    compiledString->sEndPrintf("neq ; !=\n");
                    break;
                case Lex_Gr:
                    compiledString->sEndPrintf("gr ; >\n");
                    break;
                case Lex_Le:
                    compiledString->sEndPrintf("le ; <\n");
                    break;
                default: {
                    CompileError err {};
                    err.cTor("Unknown operator in c_CmpOperator: ", head->getLexeme());
                    err.msg->sEndPrintf("%s", lexemeTypeToString(type));
                    cErrors->push(err);
                    return;
                }
            }
        }

        void c_ReturnStmt(ASTNode *head) {
            if (head->getLeft()->getKind() != Kind_None) {
                processFurther(head->getLeft(), true);
                decRex(env.getFunctionOffset());
                compiledString->sEndPrintf("pop rax ; return value\n"
                                           "ret\n");
            } else {
                decRex(env.getFunctionOffset());
                compiledString->sEndPrintf("ret\n");
            }
        }

        void c_Print(ASTNode *head) {
            processFurther(head->getLeft(), true);
            compiledString->sEndPrintf("out\n");
            compiledString->sEndPrintf("pop\n");
        }

        void c_Input(ASTNode *head) {
            compiledString->sEndPrintf("in ; input %zu:%zu \n", head->getLexeme().getLine(),
                                       head->getLexeme().getCol());
        }

        void c_IfStmt(ASTNode *head) {
            StrContainer continueLabel = getJmpLabel(head->getLexeme(), "cont");
            StrContainer elseLabel = getJmpLabel(head->getLexeme(), "else");

            ASTNode *ifBranch = head->getRight()->getLeft();
            ASTNode *elseBranch = head->getRight()->getRight();

            compiledString->sEndPrintf("; processing if:%zu:%zu\n", head->getLexeme().getLine(),
                                       head->getLexeme().getCol());
            processFurther(head->getLeft(), true);

            if (elseBranch->getKind() != Kind_None) {
                compiledString->sEndPrintf("push 0\n"
                                           "je %s\n", elseLabel.begin());
            } else {
                compiledString->sEndPrintf("push 0\n"
                                           "je %s\n", continueLabel.begin());
            }
            processFurther(ifBranch);
            if (elseBranch->getKind() != Kind_None) {
                compiledString->sEndPrintf("jmp %s\n", continueLabel.begin());
                compiledString->sEndPrintf("%s:\n", elseLabel.begin());
                processFurther(elseBranch);
            }
            compiledString->sEndPrintf("%s:\n", continueLabel.begin());
            continueLabel.dTor();
            elseLabel.dTor();
        }

        void c_None(ASTNode *head) {}

        void processFurther(ASTNode *head, bool valueNeeded = false, bool noScope = false) {
            if (head == nullptr)
                return;
            switch (head->getKind()) {
                case Kind_Linker:
                    c_Linker(head, valueNeeded, noScope);
                    break;
                case Kind_FuncDecl:
                    c_FuncDecl(head);
                    break;
                case Kind_Identifier:
                    c_Identifier(head);
                    break;
                case Kind_Number:
                    c_Number(head);
                    break;
                case Kind_AssignExpr:
                    c_AssignExpr(head);
                    break;
                case Kind_VarDef:
                    c_VarDef(head);
                    break;
                case Kind_MaUnOperator:
                    c_MaUnOperator(head);
                    break;
                case Kind_MaOperator:
                    c_MaOperator(head);
                    break;
                case Kind_Statement:
                    c_Statement(head);
                    break;
                case Kind_FuncCall:
                    c_FuncCall(head, valueNeeded);
                    break;
                case Kind_CmpOperator:
                    c_CmpOperator(head);
                    break;
                case Kind_ReturnStmt:
                    c_ReturnStmt(head);
                    break;
                case Kind_Print:
                    c_Print(head);
                    break;
                case Kind_IfStmt:
                    c_IfStmt(head);
                    break;
                case Kind_Input:
                    c_Input(head);
                    break;
                case Kind_None:
                    c_None(head);
                    break;
                    /*
                    case Kind_WhileStmt:
                        c_WhileStmt(head);
                        break;
                    */
                default: {
                    CompileError err {};
                    err.cTor("Undefined sequence: ", head->getLexeme());
                    err.msg->sEndPrintf("%s", ASTNodeKindToString(head->getKind()));
                    cErrors->push(err);
                    break;
                }
            }
        }

    public:
        void cTor() {
            tree.cTor();
            env.cTor();
            fileName = nullptr;
            parsed = nullptr;
            compiledString = StrContainer::New();
            compiledString->cTor("");
            cErrors = ClassicStack<CompileError>::New();
        }

        void dTor() {
            tree.dTor();
            env.dTor();
            parsed->DestructList();
            free(parsed);
            StrContainer::Delete(compiledString);
            ClassicStack<CompileError>::Delete(cErrors);
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

        void compile() {
            processFurther(tree.getHead());
            compiledString->sEndPrintf("mov rex %d\n"
                                       "call giveYouUp\n"
                                       "hlt\n\n", env.getLocalOffset());
        }

        bool loadFile(const char *filePath) {
            StrContainer content {};
            content.cTor();
            fileName = filePath;

            FILE *sourceCode = fopen(filePath, "rb");
            if (!sourceCode) {
                return false;
            }

            content.readFromFile(sourceCode);
            fclose(sourceCode);

            auto *res = NGG::LexParser::parse(&content);

            auto ASTParser = NGG::AST {};
            ASTParser.cTor();

            ASTParser.parse(res);

            this->tree = ASTParser;
            this->parsed = res;
            content.dTor();
            return true;
        }

        void printLexemes(const char *lexfileName) {
            FILE *file = fopen(lexfileName, "w");
            LexParser::dumpLexemes(*parsed, file);
            fclose(file);
        }

        bool isParseSuccessful() {
            return !tree.hasError();
        }

        bool isCompileSuccessful() {
            return cErrors->isEmpty();
        }

        void dumpCompileErrorStack(const char *inputFileName) {
            for (unsigned i = 0; i < cErrors->getSize(); ++i) {
                const CompileError &err = cErrors->get(i);
                printf("%s:%zu:%zu: error: %s\n", inputFileName, err.line + 1, err.col, err.msg->begin());
            }
        }

        void dumpErrorStack(const char *inputFileName) {
            tree.dumpParseErrorStack(inputFileName);
        };

        void dumpGraph() {
            FILE *file = fopen("graph.gv", "wb");
            tree.dumpTree(file);
            fclose(file);
            system("dot -Tsvg graph.gv -o code.svg");
            system("dot -Tpng graph.gv -o code.png && rm graph.gv");
        }

        StrContainer *getCompiledString() {
            return compiledString;
        }

        void saveAsmSource(const char *fileAsmName) {
            StrContainer *asmCode = this->getCompiledString();
            FILE *outFile = fopen(fileAsmName, "wb");
            fwrite(asmCode->getStorage(), 1, asmCode->getLen(), outFile);
            fclose(outFile);
        }

        static void genBytecode(const char *fileInp, CLParams &params) {
            char *buffer = nullptr;
            asprintf(&buffer, "SPUAsm %s %s --input %s --output %s %s %s",
                     params.lstFile ? "--lst" : "", params.lstFile ? params.lstFileName : "",
                     fileInp,
                     params.outputFileName,
                     params.preprocessed ? "-E" : "",
                     params.verbose ? "--verbose" : "");
            system(buffer);
            free(buffer);
        }

        void dumpTree() {
            FILE* file = fopen("treestruct.txt", "w");
            ASTLoader::dump(tree.getHead(), file);
        }
    };
}

#endif //NGG_NGG_H

//
// Created by Александр Дремов on 10.12.2020.
//

#ifndef NGG_NGG_H
#define NGG_NGG_H

#include "ScopeProcessor.h"

namespace NGG {
    class NGGCompiler {
        StrContainer *compiledString;
        AST tree;
        ScopeProcessor env;
        const char *fileName;

        void incRex(size_t num) {
            compiledString->sEndPrintf(
                    "; incrementing rex for %zu\n"
                    "push %zu\n"
                    "push rex\n"
                    "add\n"
                    "pop rex\n"
                    "; inc complete\n\n",
                    num, num);
        }

        static StrContainer getJmpLabel(Lexeme lex, const char *specifier = "") {
            StrContainer label {};
            const int randSufLen = 5;
            label.cTor(static_cast<char *>(calloc(1, sizeof(char))));

            char randSuffix[randSufLen + 1] = "";
            for (unsigned i = 0; i < randSufLen; i++)
                randSuffix[i] = (char) ('A' + rand() % 24);
            randSuffix[randSufLen] = 0;
            label.sEndPrintf("lbl_l%zu_c%zu_%s__%s", lex.getLine(), lex.getCol(), specifier, randSuffix);
            return label;
        }

        void decRex(size_t num) {
            compiledString->sEndPrintf(
                    "; decrementing rex for %zu\n"
                    "push rex\n"
                    "push %zu\n"
                    "sub\n"
                    "pop rex\n"
                    "; dec complete\n\n",
                    num, num);
        }

        void varSpecificOperation(const char *operation, IdCompiler var, int incModifier = 0) {
            if (var.type == ID_VarGlob) {
                compiledString->sEndPrintf("%s [%zu] ; identifier %s\n ",
                                           operation, var.memOffset + incModifier, var.name.getStorage());
            } else if (var.type == ID_Var) {
                compiledString->sEndPrintf("%s [rex+%zu] ; identifier %s\n",
                                           operation, var.memOffset + incModifier, var.name.getStorage());
            } else {
                // TODO: error
            }
        }

        void c_Linker(ASTNode *head, bool valueNeeded) {
            processFurther(head->getLeft(), valueNeeded);
            if (head->getRight())
                c_Linker(head->getRight(), valueNeeded);
        }

        void c_FuncDecl(ASTNode *head) {
            StrContainer label = head->getLexeme().getString();

            compiledString->sEndPrintf("\n; %s function definition (source:%zu:%zu)\n",
                                       label.getStorage(), head->getLexeme().getLine(), head->getLexeme().getCol());
            compiledString->sEndPrintf("%s:\n", label.getStorage());
            env.addNewLevel();

            if (head->getLeft()->getKind() != Kind_None) {
                ASTNode *cur = head->getLeft();
                while (cur != nullptr) {
                    Lexeme name = cur->getLeft()->getLexeme();
                    env.def(name.getString(), ID_Var);
                    cur = cur->getRight();
                }
            }

            processFurther(head->getRight());
            compiledString->sEndPrintf("ret\n\n\n");
            env.deleteLocal();
        }

        void c_Identifier(ASTNode *head) {
            StrContainer label = head->getLexeme().getString();

            Optional<IdCompiler> found = env.get(label);
            if (!found.hasValue()) {
                // TODO: error
                return;
            }

            varSpecificOperation("push", found.unwrap());
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
                default: {
                    // TODO: error
                    break;
                }
            }
        }

        void c_AssignExpr(ASTNode *head) {
            auto type = head->getLexeme().getType();

            ASTNode *idNode = head->getRight();
            ASTNode *valNode = head->getLeft();

            StrContainer name = idNode->getLexeme().getString();
            Optional<IdCompiler> found = env.get(name);

            if (!found.hasValue()) {
                // TODO: error
                return;
            }

            switch (type) {
                case Lex_AdAssg:
                    processFurther(valNode, true);
                    varSpecificOperation("push", found.unwrap());
                    compiledString->sEndPrintf("add ; Lex_AdAssg %zu:%zu\n",
                                               head->getLexeme().getLine(), head->getLexeme().getCol());
                    break;
                case Lex_MiAssg:
                    varSpecificOperation("push", found.unwrap());
                    processFurther(valNode, true);
                    compiledString->sEndPrintf("sub ; Lex_AdAssg %zu:%zu\n",
                                               head->getLexeme().getLine(), head->getLexeme().getCol());
                    break;
                case Lex_MuAssg:
                    processFurther(valNode, true);
                    varSpecificOperation("push", found.unwrap());
                    compiledString->sEndPrintf("mul ; Lex_AdAssg %zu:%zu\n",
                                               head->getLexeme().getLine(), head->getLexeme().getCol());
                    break;
                case Lex_DiAssg:
                    processFurther(valNode, true);
                    varSpecificOperation("push", found.unwrap());
                    compiledString->sEndPrintf("div ; Lex_AdAssg %zu:%zu\n",
                                               head->getLexeme().getLine(), head->getLexeme().getCol());
                    break;
                case Lex_Assg: {
                    processFurther(head->getRight(), true);
                }
                default: {
                    // TODO: error
                    break;
                }
            }
            varSpecificOperation("pop", found.unwrap());
        }

        void c_VarDef(ASTNode *head) {
            auto type = head->getLexeme();
            bool res = env.def(type.getString(), ID_Var);
            if (!res) {
                // TODO: error
            }
            processFurther(head->getLeft(), true);
            Optional<IdCompiler> found = env.get(type.getString());

            varSpecificOperation("pop", found.unwrap());
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
            compiledString->sEndPrintf("call %s\n", head->getLexeme().getString().begin());
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
                    // TODO: error
                }
            }
        }

        void c_ReturnStmt(ASTNode *head) {
            processFurther(head->getLeft(), true);
            compiledString->sEndPrintf("pop rax ; return value\n"
                                       "ret\n");
        }

        void c_Print(ASTNode *head) {
            processFurther(head->getLeft(), true);
            compiledString->sEndPrintf("out\n");
            compiledString->sEndPrintf("pop\n");
        }

        void c_Input(ASTNode *head) {
            compiledString->sEndPrintf("in ; input %zu:%zu \n", head->getLexeme().getLine(), head->getLexeme().getCol());
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
            compiledString->sEndPrintf("jmp %s\n", continueLabel.begin());
            if (elseBranch->getKind() != Kind_None) {
                compiledString->sEndPrintf("%s:\n", elseLabel.begin());
                processFurther(elseBranch);
            }
            compiledString->sEndPrintf("%s:\n", continueLabel.begin());
            continueLabel.dTor();
            elseLabel.dTor();
        }

        void processFurther(ASTNode *head, bool valueNeeded=false) {
            if (head == nullptr)
                return;
            switch (head->getKind()) {
                case Kind_Linker:
                    c_Linker(head, valueNeeded);
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
                    /*
                    case Kind_WhileStmt:
                        c_WhileStmt(head);
                        break;
                    */
                default: {
                    printf("Undefined sequence: %s\n", ASTNodeKindToString(head->getKind()));
                    break;
                }
            }
        }

    public:
        void cTor() {
            tree.cTor();
            env.cTor();
            fileName = nullptr;
            compiledString = StrContainer::New();
            compiledString->cTor(static_cast<char *>(calloc(1, sizeof(char))));
        }

        void dTor() {
            tree.dTor();
            env.dTor();
            StrContainer::Delete(compiledString);
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
            compiledString->sEndPrintf("mov rex 0\n"
                                       "call giveYouUp\n"
                                       "jmp end_label\n\n");
            processFurther(tree.getHead());
            compiledString->sEndPrintf("end_label:\n"
                                       "hlt\n");
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

            auto res = NGG::LexParser::parse(&content);

            auto ASTParser = NGG::AST {};
            ASTParser.cTor();

            ASTParser.parse(res);
            this->tree = ASTParser;
            return true;
        }

        bool isParseSuccessful() {
            return !tree.hasError();
        }

        void dumpErrorStack() {
            tree.dumpErrorStack();
        };

        void dumpGraph(FILE *file) {
            tree.dumpTree(file);
        }

        StrContainer *getCompiledString() {
            return compiledString;
        }

        void printErrors(){

        }

        void saveAsmSource(const char* fileName) {
            StrContainer* asmCode = this->getCompiledString();
            FILE* outFile = fopen(fileName, "wb");
            fwrite(asmCode->getStorage(),1, asmCode->getLen(), outFile);
            fclose(outFile);
        }

        void genBytecode(const char* fileInp, const char* fileOut){
            char* buffer = nullptr;
            asprintf(&buffer, "SPUAsm --lst listing.lst --verbose --input %s --output %s", fileInp, fileOut);
            system(buffer);
            free(buffer);
        }
    };
}

#endif //NGG_NGG_H

//
// Created by Александр Дремов on 11.12.2020.
//

#ifndef NGG_PARAMSPARSER_H
#define NGG_PARAMSPARSER_H

#define MAXPATHLEN 1024

struct CLParams {
    const char* inputFileName;
    char* inputFileRealName;
    const char* outputFileName;

    const char* lstFileName;
    bool lstFile;

    char* codeText;
    bool  verbose;
    bool  preprocessed;
    bool  keepCode;
    bool  graph;
    bool  lexemes;
    bool  dumpGraph;
    const char* lexFileName;

    static void printHelpData(){
        printf("NGG help\n"
               "--input     <input file> input file to be compiled .ngg format (source)\n"
               "--output    <output file> output file. output.spub by default (spu binary)\n"
               "-h, --help  show this help message\n"
               "--verbose   output debug information to the console\n"
               "--lst       <.lst file> file to output .lst assembly data assembly.lst by default\n"
               "--lex       <.lex file> file to dump lexemes\n"
               "-E          generate preprocessed file assembly.spuprep\n"
               "-o          keep temporary asm code\n"
               "-g          generate AST graph\n"
               "-d          dump AST graph\n"
               "\n");
    }

    bool parseArgs(int argc, const char* argv[]) {
        if (argc <= 1){
            printHelpData();
            return false;
        }

        this->inputFileRealName = (char*)calloc(MAXPATHLEN, 1);

        for(int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "--input") == 0) {
                if (i + 1 > argc) {
                    printf("error: assembly: No input file specified after --input\n");
                    return false;
                }
                this->inputFileName = *(argv + i + 1);
                realpath(this->inputFileName, this->inputFileRealName);
                i++;
            }else if (strcmp(argv[i], "--output") == 0) {
                if (i + 1 > argc) {
                    printf("error: assembly: No output file specified after --output\n");
                    return false;
                }
                this->outputFileName = *(argv + i + 1);
                i++;
            }else if (strcmp(argv[i], "--verbose") == 0) {
                this->verbose = true;
            }else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
                printHelpData();
            }else if (strcmp(argv[i], "--lst") == 0) {
                if (i + 1 > argc){
                    printf("error: assembly: No lstfile file specified after --lst\n");
                    return false;
                }
                lstFile = true;
                this->lstFileName = *(argv + i + 1);
                i++;
            }else if (strcmp(argv[i], "-E") == 0) {
                this->preprocessed = true;
            }else if (strcmp(argv[i], "-d") == 0) {
                this->dumpGraph = true;
            }else if (strcmp(argv[i], "-g") == 0) {
                this->graph = true;
            }else if (strcmp(argv[i], "--lex") == 0) {
                if (i + 1 > argc){
                    printf("error: assembly: No lex file specified after --lex\n");
                    return false;
                }
                lexemes = true;
                this->lexFileName = *(argv + i + 1);
                i++;
            }else if (strcmp(argv[i], "-o") == 0) {
                this->keepCode = true;
            } else {
                if (this->inputFileName == nullptr){
                    this->inputFileName = *(argv + i);
                    realpath(this->inputFileName, this->inputFileRealName);
                }
            }
        }

        if (this->inputFileName == nullptr) {
            printf("error: assembly: No input file specified\n");
            return false;
        }

        if (this->outputFileName == nullptr) {
            this->outputFileName = "output.spub";
        }

        if (this->lstFileName == nullptr) {
            this->lstFileName = "assembly.lst";
        }
        return true;
    }

    void cTor() {
        inputFileName = nullptr;
        inputFileRealName = nullptr;
        outputFileName = nullptr;

        lstFileName = nullptr;
        lstFile = false;

        codeText = nullptr;
        verbose = false;
        preprocessed = false;
        keepCode = false;
        graph = false;
        lexemes = false;
        lexFileName = nullptr;
        dumpGraph = false;
    }

    void dTor() {
        free(inputFileRealName);
    }
};

#endif //NGG_PARAMSPARSER_H

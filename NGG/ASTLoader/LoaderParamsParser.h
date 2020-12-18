//
// Created by Александр Дремов on 11.12.2020.
//

#ifndef NGG_LOADPARAMSPARSER_H
#define NGG_LOADPARAMSPARSER_H

#define MAXPATHLEN 1024

struct LoaderCLParams {
    const char* inputFileName;
    char* inputFileRealName;
    const char* outputFileName;

    bool  graph;

    static void printHelpData(){
        printf("NGG help\n"
               "--input     <input file> input file to be used (tree dumped)\n"
               "--output    <output file> output file. output.ngg by default (ngg source)\n"
               "-h, --help  show this help message\n"
               "-g          generate AST graph\n"
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
            }else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
                printHelpData();
            }else if (strcmp(argv[i], "-g") == 0) {
                this->graph = true;
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
            this->outputFileName = "output.ngg";
        }
        return true;
    }

    void cTor() {
        inputFileName = nullptr;
        inputFileRealName = nullptr;
        outputFileName = nullptr;
        graph = false;
    }

    void dTor() {
        free(inputFileRealName);
    }
};

#endif //NGG_PARAMSPARSER_H

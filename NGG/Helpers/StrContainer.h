//
// Created by Александр Дремов on 09.12.2020.
//

#ifndef NGG_STRCONTAINER_H
#define NGG_STRCONTAINER_H
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cstdarg>

class StrContainer {
    char*  storage;
    size_t len;
    size_t maxLen;

    void reallocate(size_t additional = 1) {
        if (len + additional >= maxLen){
            storage = static_cast<char*>(realloc(storage, maxLen + additional + 1));
            maxLen += additional;
        }
    }

public:
    void cTor(){
        storage = nullptr;
        len = 0;
        maxLen = 0;
    }

    void cTor(const char* line){
        len = strlen(line);
        storage = strdup(line);
        maxLen = len;
    }

    void cTor(const char* line, size_t slen){
        len = slen;
        storage = static_cast<char*>(calloc(len + 1, 1));
        for(size_t i =0; i < slen; ++i) storage[i] = line[i];
        storage[slen] = '\0';
        maxLen = len;
    }

    void dTor(){
        if (storage) {
            free(storage);
            storage = reinterpret_cast<char *>(0xBADF);
        }
    }

    static StrContainer* New(){
        auto* ob = static_cast<StrContainer*>(calloc(1, sizeof(StrContainer)));
        ob->cTor();
        return ob;
    }

    static void Delete(StrContainer* ob){
        ob->dTor();
        free(ob);
    }

    [[nodiscard]] const char* getStorage() const {
        return const_cast<const char*>(storage);
    }

    [[nodiscard]] char* getStorage() {
        return storage;
    }

    [[nodiscard]] size_t getLen() const {
        return len;
    }

    void updateLen(){
        len = strlen(storage);
    }

    void append(char symbol) {
        reallocate(5);
        storage[len++] = symbol;
        storage[len] = '\0';
    }

    void append(StrContainer* str) {
        reallocate(str->len + 1);
        strcpy(storage + len, str->storage);
        len += str->len;
        storage[len] = '\0';
        updateLen();
    }

    void sEndPrintf(const char * format, ...) {
        if (!isValid()){
            printf("Invalid storage");
            return;
        }
        va_list args;
        va_start (args, format);

        char* lineAdd = nullptr;
        vasprintf(&lineAdd, format, args);
        size_t additionalSpace = strlen(lineAdd);

        reallocate(additionalSpace);
        sprintf (storage + len, "%s", lineAdd);

        free(lineAdd);
        va_end (args);
        updateLen();
    }

    [[nodiscard]] char* begin() const {
        return storage;
    }

    [[nodiscard]] char* end() const {
        return storage + len;
    }

    void readFromFile(FILE* file){
        fseek(file, 0, SEEK_END);
        size_t fsize = ftell(file);
        fseek(file, 0, SEEK_SET);

        reallocate(fsize + 10);
        fread(storage, 1, fsize, file);

        storage[fsize] = 0;
        updateLen();
    }

    bool isValid(){
        return storage != (char*) 0xBADF;
    }
};

#endif //NGG_STRCONTAINER_H

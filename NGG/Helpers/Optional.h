//
// Created by Александр Дремов on 09.12.2020.
//

#ifndef NGG_OPTIONAL_H
#define NGG_OPTIONAL_H
#include <cstdlib>

template<typename T>
class Optional {
    T      _value;
    bool   _hasValue;
public:
    void cTor(T value){
        _value = value;
        _hasValue = true;
    }

    void cTor(){
        _hasValue = false;
    }

    void dTor(){}

    static Optional* New(){
        auto* ob = static_cast<Optional*>(calloc(1, sizeof(Optional)));
        ob->cTor();
        return ob;
    }

    static void Delete(Optional* ob){
        ob->dTor();
        free(ob);
    }

    bool hasValue(){
        return _hasValue;
    }

    T unwrap() {
        return _value;
    }

    T* operator->(){
        return &this->_value;
    }
};

#endif //NGG_OPTIONAL_H

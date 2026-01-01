#pragma once

#include "CoreMinimal.h"

template <typename T>
class COREMATH_API TIndexedElement {

public:
    TIndexedElement() {};
    TIndexedElement(T in, int i)
    {
        type = in;
        index = i;
    }
    ~TIndexedElement(){}

    TIndexedElement(const TIndexedElement &other){
        if(this != &other){
            *this = other;
        }
    }

    TIndexedElement &operator=(const TIndexedElement &other){
        if(this != &other){
            index = other.index;
            type = other.type;
        }
        return *this;
    }

    T Get() const {
        return type;
    }

    int GetIndex() const {
        return index;
    }

private:
    int index = 0;
    T type;
};
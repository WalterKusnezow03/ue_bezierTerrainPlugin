#pragma once

#include "CoreMinimal.h"


class COREMATH_API IPayloadInterface {

public:
    IPayloadInterface(){}
    ~IPayloadInterface(){}

    virtual void Notify(FString message){
        //to be implemented
    };

};
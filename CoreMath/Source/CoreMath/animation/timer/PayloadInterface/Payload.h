#pragma once

#include "CoreMinimal.h"

#include "IPayLoadInterface.h"

class COREMATH_API Payload {

public:
    Payload();
    ~Payload();

    Payload(const Payload &other);
    Payload &operator=(const Payload &other);

    
    Payload(IPayloadInterface *interfaceIn, FString messageIn);
    void Setup(IPayloadInterface *interfaceIn, FString messageIn);
    void Execute();

private:
    IPayloadInterface *interface = nullptr;
    FString message;
};
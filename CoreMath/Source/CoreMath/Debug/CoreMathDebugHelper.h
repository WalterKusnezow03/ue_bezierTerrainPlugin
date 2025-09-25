#pragma once

#include "CoreMinimal.h"

class COREMATH_API CoreMathDebugHelper {

public:

    static void logMessage(FString printing){
        UE_LOG(LogTemp, Log, TEXT("%s"), *printing); //for whatever reason the string must be dereferenced
    }

    static void logMessage(FString s, FString t){
        s += t;
        logMessage(s);
    }

private:
    CoreMathDebugHelper() {};
    ~CoreMathDebugHelper() {};
};
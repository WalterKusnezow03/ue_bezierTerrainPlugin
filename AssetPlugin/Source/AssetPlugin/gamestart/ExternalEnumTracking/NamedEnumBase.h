#pragma once

#include "CoreMinimal.h"

/// @brief base interface for named enums
struct ASSETPLUGIN_API FNamedEnumBase {

    //to be overriden.
    virtual ~FNamedEnumBase(){

    }

    FString name = TEXT("invalidName");

    void SetName(FString s){
        if(s.Len() > 0){
            name = s;
        }
    }

    FString GetName(){
        return name;
    }
};

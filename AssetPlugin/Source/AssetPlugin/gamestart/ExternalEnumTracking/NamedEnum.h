#pragma once

#include "CoreMinimal.h"
#include "NamedEnumBase.h"

/// @brief will track a set of enum values and give it a explicit name
/// @tparam E 
template <typename E>
struct ASSETPLUGIN_API FNamedEnum : public FNamedEnumBase{

    static_assert(std::is_enum<E>::value, "must be an enum");

    //variables
    TArray<E> tracked;

    //methods

    //destructor override
    ~FNamedEnum(){
        tracked.Empty(); //not needed but destructor override is.
    }

    //constructor
    FNamedEnum(FString nameIn){
        SetName(nameIn);
        UEnum* EnumPtr = StaticEnum<E>();
        if (EnumPtr){
            for(int32 i = 0; i < EnumPtr->NumEnums() - 1; ++i){
                tracked.Add(static_cast<E>(EnumPtr->GetValueByIndex(i)));
            }
        }
    }


    //selected enum values
    FNamedEnum(FString nameIn, TArray<E> &trackedIn){
        SetName(nameIn);
        SetEnumTracked(trackedIn);
    }
    
    void SetEnumTracked(TArray<E> &trackedIn){
        tracked = trackedIn;
    }

    bool Contains(E e){
        return tracked.Contains(e);
    }

    //make key if contains!
    FString makeKey(E e){
        FString key = name;
        int32 Index;
        if (tracked.Find(e, Index)){
            key += FString::Printf(TEXT("index%d"), Index);
        }
        return key;
    }
};
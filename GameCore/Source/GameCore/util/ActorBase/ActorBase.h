#pragma once  
#include "CoreMinimal.h" 
#include "GameCore/util/AActorUtil.h"
#include "ActorBase.generated.h"  

UCLASS() 
class GAMECORE_API AActorBase : public AActor {
    GENERATED_BODY()

public:

    void FindAllChildsByName(FString namepart, TArray<USceneComponent *> &container);
    void FindAllChildsByName(
        USceneComponent *comp,
        FString namepart,
        TArray<USceneComponent *> &container
    );


    template <typename T>
    void TFindAllChildsByName(FString name, TArray<T*> &outarray){
        TArray<USceneComponent *> found;
        FindAllChildsByName(name, found);
        for (int i = 0; i < found.Num(); i++){
            if(USceneComponent *current = found[i]){
                if(T* castedComponent = Cast<T>(current)){
                    outarray.Add(castedComponent);
                }
            }
        }
    }

    //is tested
    template <typename T>
    void TTryAssignByName(FString name, T *&ptrToAssign){
        TArray<T *> array;
        TFindAllChildsByName(name, array);
        for (int i = 0; i < array.Num(); i++){
            if(T *current = array[i]){
                if(USceneComponent *casted = Cast<T>(current)){
                    if(casted->GetName() == name){
                        ptrToAssign = current;
                        return;
                    }
                }
            }
        }
    }


    //find all by type
    template <typename T>
    void TFindAllChildsByType(
        TArray<T *> &container
    ){
        TArray<USceneComponent *> raw;
        FindAllChilds(raw);

        TArray<T *> casted;
        for (int i = 0; i < raw.Num(); i++){
            if(USceneComponent *current = raw[i]){
                if(T *currentCasted = Cast<T>(current)){
                    casted.Add(currentCasted);
                }
            }
        }
    }


    void FindAllChilds(TArray<USceneComponent *> &container);
    void FindAllChilds(
        USceneComponent *comp,
        TArray<USceneComponent *> &container
    );

private:

    


};
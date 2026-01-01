#include "ActorBase.h"


void AActorBase::FindAllChildsByName(FString namepart, TArray<USceneComponent*> &container){
    
    
    //check all child for being actos
    TArray<USceneComponent *> childs;
    GetComponents<USceneComponent>(childs);
    for (int i = 0; i < childs.Num(); i++){
        FindAllChildsByName(childs[i], namepart, container);
    }
}


void AActorBase::FindAllChildsByName(
    USceneComponent *comp, 
    FString namepart, 
    TArray<USceneComponent*> &container
){
    if(comp){
        TArray<USceneComponent *> childs = comp->GetAttachChildren();
        for (int i = 0; i < childs.Num(); i++)
        {
            if(childs[i] != nullptr){
                
                FString currName = childs[i]->GetName();
                if(currName.Contains(namepart)){
                    container.Add(childs[i]);
                }
                FindAllChildsByName(childs[i], namepart, container);
            }
        }
    }
    
}



void AActorBase::FindAllChilds(TArray<USceneComponent *> &components){
    TArray<USceneComponent *> raw;
    GetComponents<USceneComponent>(raw);
    for (int i = 0; i < raw.Num(); i++){
        if(USceneComponent *current = raw[i]){
            FindAllChilds(current, components);
        }
    }
}



void AActorBase::FindAllChilds(
    USceneComponent *comp, 
    TArray<USceneComponent*> &container
){
    if(comp){
        TArray<USceneComponent *> childs = comp->GetAttachChildren();
        for (int i = 0; i < childs.Num(); i++)
        {
            if(USceneComponent *current = childs[i]){
                container.Add(current);
                
                FindAllChilds(current, container);
            }
        }
    }
        
}
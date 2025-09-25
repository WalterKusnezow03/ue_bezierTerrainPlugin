#include "worldLevelBase.h"

AworldLevelBase *AworldLevelBase::Instance = nullptr;

AworldLevelBase::AworldLevelBase(){
    PrimaryActorTick.bCanEverTick = true; 
}

void AworldLevelBase::MakeInstanceBase(UWorld *world){
    if(!Instance && world){
        AworldLevelBase *made = Make<AworldLevelBase>(world);
    }
}

void AworldLevelBase::BeginPlay(){
    Instance = this;
}

void AworldLevelBase::Tick(float deltatime){
    Super::Tick(deltatime);
}

void AworldLevelBase::EndPlay(const EEndPlayReason::Type EndPlayReason){
    Instance = nullptr;
    Super::EndPlay(EndPlayReason);
}


void AworldLevelBase::addOutpostAt(FVector &pos){
    if(Instance){
        Instance->outpostsToCreate.Add(pos);
    }
}


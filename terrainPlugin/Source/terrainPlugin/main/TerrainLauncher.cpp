#include "TerrainLauncher.h"
#include "terrainPlugin/AssetEditorCreation/TerrainPluginAssetLoader.h"

ATerrainLauncher::ATerrainLauncher(){
    PrimaryActorTick.bCanEverTick = true; //needed for tick update
}

ATerrainLauncher* ATerrainLauncher::makeInstance(UWorld *world, FString WorldLevelName){
    TerrainPluginAssetLoader::LoadGrassAssetOnBeginPlay(); //happens only once anyway.
    if(world != nullptr){

        UClass *toSpawn = ATerrainLauncher::StaticClass();
        if(toSpawn){
            
            FActorSpawnParameters SpawnParams;
            FVector Location;
            AActor *spawned = world->SpawnActor<AActor>(toSpawn, Location, FRotator::ZeroRotator, SpawnParams); 
            if(spawned){

                ATerrainLauncher *casted = Cast<ATerrainLauncher>(spawned);
                if(casted){
                    casted->BeginAndLoad(WorldLevelName);
                    return casted;
                }
            }
        }
    }
    return nullptr;
}

void ATerrainLauncher::BeginPlay(){
    Super::BeginPlay();
    
}

// Override EndPlay
void ATerrainLauncher::EndPlay(const EEndPlayReason::Type EndPlayReason){
    //super endplay
    EndAndSave();
    Super::EndPlay(EndPlayReason);
}

void ATerrainLauncher::Tick(float deltatime){
    Super::Tick(deltatime);
    actorManager.Tick(deltatime);
}



// -- external begin / end api --

void ATerrainLauncher::BeginAndLoad(FString WorldLevelName){
    actorManager.BeginPlay(WorldLevelName, GetWorld());
}

void ATerrainLauncher::EndAndSave(){
    //actor manager save data
    actorManager.EndPlay();
}


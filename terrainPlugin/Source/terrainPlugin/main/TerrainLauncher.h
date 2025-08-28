#pragma once

#include "CoreMinimal.h"
#include "terrainPlugin/main/management/ActorManager.h"
#include "TerrainLauncher.generated.h"

/// @brief will launch the terrain generation or try to load from save storage
UCLASS()
class TERRAINPLUGIN_API ATerrainLauncher : public AActor{

    GENERATED_BODY()

public:
    ATerrainLauncher();

    static ATerrainLauncher* makeInstance(UWorld *world, FString WorldLevelName);

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void Tick(float deltatime) override;

    //external play and end - does not kill this instance, aactors can be reused for loading another level!
    void BeginAndLoad(FString WorldLevelName);
    void EndAndSave();

private:
    

    ActorManager actorManager;
};
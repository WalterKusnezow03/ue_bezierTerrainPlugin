#pragma once

#include "CoreMinimal.h"
#include "CoreMath/animation/TargetInterpolator.h"
#include "GameCore/interfaces/Interactinterface.h"
#include "terrainPlugin/meshgen/customMeshActor.h"
#include "DoorBase.generated.h"


UCLASS()
class TERRAINPLUGIN_API ADoorBase : public AcustomMeshActor , public IInteractinterface {//ggf interact interface
    GENERATED_BODY()

public:
    ADoorBase();

    static ADoorBase *Construct(UWorld *world, FVector &location);

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    void open();
    void close();

    virtual void interact() override;

protected:
    virtual void initMesh();

    bool canChangeStateNow();

    bool bIsOpenState = false;

    TargetInterpolator interpolator; //es braucht eine rotator direction

    float timeOfAnimation = 0.5f;
};
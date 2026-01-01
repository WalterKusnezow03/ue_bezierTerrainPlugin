#include "MovingFoliageInstancerComponent.h"
#include "DebugPlugin/DebugHelper.h"

void UMovingFoliageInstancerComponent::Init(int countChilds, UStaticMesh *someMesh, AActor *parent){
    if(bIsdisbaled){
        return;
    }

    if(!instancer && someMesh && parent){
        //UInstancedStaticMeshComponent isA UStaticMeshComponent
        instancer = NewObject<UInstancedStaticMeshComponent>(this);

        //UStaticMeshComponent::SetStaticMesh(UStaticMesh* NewMesh)
        instancer->SetStaticMesh(someMesh);
        instancer->RegisterComponent();
        //instancer->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
        //this->AttachToComponent(parent->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
        
        //instancer = NewObject<UInstancedStaticMeshComponent>(parent);
        
        instancer->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        instancer->AttachToComponent(parent->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
        SetHiddenInGame(false);

        FTransform empty;
        for (int i = 0; i < countChilds; i++)
        {   
            //UInstancedStaticMeshComponent::AddInstanceById(FTransform, UStaticMesh)
            instancer->AddInstance(
                empty,
                bWorldSpaceFlag
            );
            //AddedChilds.Add(newChild);
            transformChilds.Add(empty);
        }

        parentPtr = parent;
    }
}

void UMovingFoliageInstancerComponent::Update(
    const TArray<FVector> &positions,
    const TArray<FVector> &normals,
    ELod lodLevelcurrent
){
    if(bIsdisbaled){
        return;
    }

    if(instancer){
        if(lodLevelcurrent != ELod::lodNear){
            SetHiddenInGame(true);
            return;
        }
        if(positions.Num() <= 0){
            SetHiddenInGame(true);
            return;
        }

        SetHiddenInGame(false);
        UpdateTransformArray(positions, normals);
        int32 StartInstanceIndex = 0;
        bool bMarkRenderStateDirty = true;
        bool bTeleport = true;
        instancer->BatchUpdateInstancesTransforms(
            StartInstanceIndex,
            transformChilds,
            bWorldSpaceFlag,
            bMarkRenderStateDirty,
            bTeleport
        );
        
    }
    
    

    /*
    BatchUpdateInstancesTransforms (
        int32 StartInstanceIndex,
        const TArray< FTransform >& NewInstancesTransforms,
        bool bWorldSpace,
        bool bMarkRenderStateDirty,
        bool bTeleport
    )

    UpdateInstanceTransformById (
        FPrimitiveInstanceId InstanceId,
        const FTransform& NewInstanceTransform,
        bool bWorldSpace,
        bool bTeleport
    )
    */
}


void UMovingFoliageInstancerComponent::UpdateTransformArray(
    const TArray<FVector> &positions,
    const TArray<FVector> &normals
){
    if(bIsdisbaled){
        return;
    }

    int limit = std::min(positions.Num(), transformChilds.Num());
    for (int i = 0; i < limit; i++)
    {
        FTransform &current = transformChilds[i];
        current.SetTranslation(positions[i]);
        /*DebugHelper::logMessage(
            FString::Printf(TEXT("UMovingFoliageInstancerComponent update transform %s"), *positions[i].ToString())
        );

        if(parentPtr){
            DebugHelper::showLineBetween(
                parentPtr->GetWorld(),
                parentPtr->GetActorLocation() + positions[i],
                parentPtr->GetActorLocation() + positions[i] + FVector(0,0,50000),
                FColor::Red,
                1000.0f
            );
        }*/
    }
    //make others dissappear
    if(limit < transformChilds.Num()){
        for (int i = limit; i < transformChilds.Num(); i++)
        {
            FVector pos(0, 0, -100000);
            FTransform &current = transformChilds[i];
            current.SetTranslation(pos);
        }
    }

    //apply rotation
    int normallimit = std::min(normals.Num(), transformChilds.Num());
    for (int n = 0; n < normallimit; n++){

        //TTransform::SetRotation ( const TQuat< T >& NewRotation)
        FQuat asQaut = RotataionForSurfaceNormal(normals[n]);
        FTransform &transform = transformChilds[n];
        transform.SetRotation(asQaut);



        /*
        if(parentPtr && n < positions.Num()){
            DebugHelper::showLineBetween(
                parentPtr->GetWorld(),
                parentPtr->GetActorLocation() + positions[n],
                parentPtr->GetActorLocation() + positions[n] + normals[n] * 100,
                FColor::Red,
                1000.0f
            );
        }*/
    }
}

FQuat UMovingFoliageInstancerComponent::RotataionForSurfaceNormal(const FVector &normal){
    FRotator asRotation = normal.Rotation();
    asRotation.Pitch -= 90.0f; 
    //the x axis looks forward but normal logically UP! Towards Z!
    //Rotate x axis to look up is +90 counter clock wise!, thats what i thought, its the other way around.

    FQuat asQaut(asRotation);
    return asQaut;
}

/// @brief moves all instances away and hides this component
void UMovingFoliageInstancerComponent::HideAll(){
    SetHiddenInGame(true);
}

void UMovingFoliageInstancerComponent::SetHiddenInGame(bool flag){
    if(instancer){
        instancer->SetVisibility(!flag, true); // Hide in game (and also affect children)
        
    }
}
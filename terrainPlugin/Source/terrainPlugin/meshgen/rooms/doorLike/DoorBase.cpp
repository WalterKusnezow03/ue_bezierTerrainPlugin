#include "DoorBase.h"


ADoorBase *ADoorBase::Construct(UWorld *world, FVector &location){
    if(world){
        FRotator rotation;
        FActorSpawnParameters params;
        ADoorBase *SpawnedActor = world->SpawnActor<ADoorBase>(
            ADoorBase::StaticClass(),
            location,
            FRotator::ZeroRotator,
            params
        );
        return SpawnedActor;
    }
    return nullptr;
}






ADoorBase::ADoorBase() : AcustomMeshActor() {

}

void ADoorBase::BeginPlay(){
    Super::BeginPlay();
    initMesh();
}

void ADoorBase::Tick(float DeltaTime){
    Super::Tick(DeltaTime);
    if(!canChangeStateNow()){
        FRotator rot = interpolator.interpolateRotationOnly(DeltaTime);
        SetActorRotation(rot);

        if(interpolator.hasReachedTarget()){
            bIsOpenState = !bIsOpenState; //flip
        }
    }
}

void ADoorBase::initMesh(){
    //create mesh here

    MeshData &doorMesh = Super::findMeshDataReference(
        materialEnum::stoneMaterial,
        ELod::lodNear,
        true
    );

    int scaleX = 5;
    int scaleY = 150;
    int scaleZ = 300;
    FVector pivot(0, 0, 0);
    // pivot at bottom left
    doorMesh.appendCubeAt(
        pivot,
        scaleX,
        scaleY,
        scaleZ
    );

    //Super
    ReloadMeshAndApplyAllMaterials();
}

void ADoorBase::interact(){
    if(bIsOpenState){
        close();
    }
    else
    {
        open();
    }
}



void ADoorBase::open(){
    if(canChangeStateNow()){
        FRotator currentRotation = GetActorRotation();
        FRotator nextState = currentRotation;
        nextState.Yaw += 90.0f;

        interpolator.setNewTimeToFrame(timeOfAnimation);
        interpolator.overrideStart(currentRotation);
        interpolator.overrideTarget(nextState);
        
    }
}

void ADoorBase::close(){
    if(canChangeStateNow()){

        FRotator currentRotation = GetActorRotation();
        FRotator nextState = currentRotation;
        nextState.Yaw -= 90.0f;

        interpolator.setNewTimeToFrame(timeOfAnimation); // resets reached flag too
        interpolator.overrideStart(currentRotation);
        interpolator.overrideTarget(nextState);

    }
}



bool ADoorBase::canChangeStateNow(){
    return interpolator.hasReachedTarget();
}
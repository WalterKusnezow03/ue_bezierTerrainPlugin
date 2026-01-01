#include "FProcMeshCollisionStorageInterface.h"

FProcMeshCollisionStorageInterface::FProcMeshCollisionStorageInterface(){
    wasSerialized = false;
    wasRead = false;
}

FProcMeshCollisionStorageInterface::~FProcMeshCollisionStorageInterface(){

}

FProcMeshCollisionStorageInterface::FProcMeshCollisionStorageInterface(
    const FProcMeshCollisionStorageInterface& other
){
    if(this != &other){
        *this = other;
    }
}

FProcMeshCollisionStorageInterface &FProcMeshCollisionStorageInterface::operator=(
    const FProcMeshCollisionStorageInterface &other
){
    if(this != &other){
        CollisionBinary = other.CollisionBinary;
        wasRead = other.wasRead;
        wasSerialized = other.wasSerialized;
    }
    return *this;
}

bool FProcMeshCollisionStorageInterface::HasCachedCollisionData()
{
    return wasSerialized || wasRead;
}

bool FProcMeshCollisionStorageInterface::SerializeCollision(UProceduralMeshComponent* ProcMesh){
    if (!ProcMesh) return false;

    UBodySetup* Setup = ProcMesh->GetBodySetup();
    if (!Setup) return false;

    FMemoryWriter Writer(CollisionBinary, true);
    Setup->Serialize(Writer);
    wasSerialized = true;
    return true;
}

bool FProcMeshCollisionStorageInterface::ApplyData(UProceduralMeshComponent *ProcMesh){
    if(!HasCachedCollisionData()){
        return false;
    }

    if(ProcMesh){
        if (CollisionBinary.Num() == 0) return false;

        // Neues BodySetup erstellen
        UBodySetup* NewSetup = NewObject<UBodySetup>(ProcMesh);
        if (!NewSetup) return false;

        // Byte-Array einlesen
        FMemoryReader Reader(CollisionBinary, true);
        NewSetup->Serialize(Reader);

        // BodyInstance initialisieren
        FInitBodySpawnParams SpawnParams(ProcMesh);
        ProcMesh->BodyInstance.InitBody(
            NewSetup,
            ProcMesh->GetComponentTransform(),
            ProcMesh,
            ProcMesh->GetWorld()->GetPhysicsScene(),
            SpawnParams
        );

        // Optional: Bounds aktualisieren
        ProcMesh->UpdateBounds();

        return true;
    }
    return false;
}


void FProcMeshCollisionStorageInterface::SetupFromBinary(TArray<uint8> &CollisionBinaryIn){
    if(CollisionBinaryIn.Num() > 0){
        CollisionBinary = CollisionBinaryIn;
        wasRead = true;
        return;
    }
    wasRead = false;
}
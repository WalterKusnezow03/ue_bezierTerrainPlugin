#pragma once

#include "CoreMinimal.h"
#include "PhysicsEngine/BodySetup.h"
#include "GameCore/MeshGenBase/ProceduralMeshComponentDerived/ProceduralMeshComponentCustom.h"
#include "PhysicsEngine/BodyInstance.h"

///@brief Helper class for Procedural Mesh Collision Serialization
class GAMECORE_API FProcMeshCollisionStorageInterface
{
public:
    FProcMeshCollisionStorageInterface();
    ~FProcMeshCollisionStorageInterface();

    FProcMeshCollisionStorageInterface(const FProcMeshCollisionStorageInterface& other);
    FProcMeshCollisionStorageInterface &operator=(const FProcMeshCollisionStorageInterface &other);

    /// @brief returns if collsion data was setup from component or read from storage
    /// @return 
    bool HasCachedCollisionData();

    bool SerializeCollision(UProceduralMeshComponent *ProcMesh);
    
    bool ApplyData(UProceduralMeshComponent *ProcMesh);
    

    void SetupFromBinary(TArray<uint8> &CollisionBinaryIn);

private:
    TArray<uint8> CollisionBinary;
    bool wasRead = false;
    bool wasSerialized = false;

    /** 
     * Serialisiert die gecookte Collision eines ProceduralMeshComponent in ein Byte-Array 
     * /
    static bool SerializeCollision(UProceduralMeshComponent* ProcMesh, TArray<uint8>& OutData)
    {
        if (!ProcMesh) return false;

        UBodySetup* Setup = ProcMesh->GetBodySetup();
        if (!Setup) return false;

        FMemoryWriter Writer(OutData, true);
        Setup->Serialize(Writer);
        return true;
    }

    / ** 
     * Deserialisiert gecookte Collision aus Byte-Array und initialisiert BodyInstance
     * /
    static bool DeserializeCollision(UProceduralMeshComponent* ProcMesh, const TArray<uint8>& Data)
    {
        if (!ProcMesh || Data.Num() == 0) return false;

        // Neues BodySetup erstellen
        UBodySetup* NewSetup = NewObject<UBodySetup>(ProcMesh);
        if (!NewSetup) return false;

        // Byte-Array einlesen
        FMemoryReader Reader(Data, true);
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
    }*/
};
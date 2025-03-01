// Fill out your copyright notice in the Description page of Project Settings.


#include "customWaterActor.h"


AcustomWaterActor::AcustomWaterActor() : AcustomMeshActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    meshInited = false;
}

void AcustomWaterActor::BeginPlay(){
    Super::BeginPlay();
}


void AcustomWaterActor::Tick(float DeltaTime){
    Super::Tick(DeltaTime);
    if(meshInited){
        updateRunningTime(DeltaTime);
        vertexShader();
    }
    
}

void AcustomWaterActor::updateRunningTime(float deltaTime){
    runningTime += deltaTime;
    if(runningTime > 2 * M_PI){
        runningTime = 0.0f;
    }
}



/// @brief 
/// @param sizeX in vertecies
/// @param sizeY in vertecies
/// @param detail detail between vertecies (in cm)
void AcustomWaterActor::createWaterPane(int vertexCountXIn, int vertexCountYIn, int detail){
    if(meshInited){
        return;
    }

    vertexcountX = std::abs(vertexCountXIn);
    vertexcountY = std::abs(vertexCountYIn);
    if(vertexcountX <= 2){
        vertexcountX = 3;
    }
    if(vertexcountY <= 2){
        vertexcountY = 3;
    }

    MeshData &waterMesh = findMeshDataReference(
        materialEnum::waterMaterial,
        ELod::lodNear,
        false
    );

    for (int i = 0; i < vertexcountX; i++){
        for (int j = 0; j < vertexcountY; j++){
            int xOffset = i * detail;
            int yOffset = j * detail;
            /*
            1 2
            0 3
            */
            FVector v0 = FVector(xOffset, yOffset, 0);
            FVector v1 = FVector(xOffset, yOffset + detail, 0);
            FVector v2 = FVector(xOffset + detail, yOffset + detail, 0);
            FVector v3 = FVector(xOffset + detail, yOffset, 0);

            waterMesh.appendEfficent(v0, v1, v2, v3);
        }
    }
    waterMesh.calculateNormals();

    ReloadMeshAndApplyAllMaterials();
    meshInited = true;
}


void AcustomWaterActor::vertexShader(){
    MeshData &waterMesh = findMeshDataReference(
        materialEnum::waterMaterial,
        ELod::lodNear,
        false
    );

    int layer = layerByMaterialEnum(materialEnum::waterMaterial);

    //raycast is not blocked by water
    if(MeshNoRaycast){
        TArray<FVector> &vertecies = waterMesh.getVerteciesRef();
        for (int i = 0; i < vertecies.Num(); i++){
            FVector &vertex = vertecies[i];
            applyCurve(vertex);
        }

        refreshMesh(*MeshNoRaycast, waterMesh, layer);
    }
}


/// @brief apply vertex shader to the given vertex
/// @param vertex vertex to move
void AcustomWaterActor::applyCurve(FVector &vertex){
    
    float frequency = 0.001f; // Wellenbreite
    float amplitude = 25.0f; // Wellenhöhe
    float speed = 1.0f; // Wellengeschwindigkeit //1.0;
    float wave = sin(vertex.X * frequency + runningTime * speed) + cos(vertex.Y * frequency + runningTime * speed);
    
    vertex.Z = wave * amplitude;

}



void AcustomWaterActor::refreshMesh(
    UProceduralMeshComponent& meshComponent,
    MeshData &other,
    int layer
){
    if(meshInited){
        meshComponent.UpdateMeshSection(
            layer, 
            other.getVerteciesRef(), 
            other.getNormalsRef(), 
            other.getUV0Ref(), 
            other.getVertexColorsRef(), 
            other.getTangentsRef()
        );
    }
}
// Fill out your copyright notice in the Description page of Project Settings.
#include "MeshExtractor.h"


#include "CoreMinimal.h"
#include "EngineUtils.h"
#include "Engine/StaticMesh.h"
#include "StaticMeshResources.h" 
#include "GameFramework/Actor.h"

#include "Engine/StaticMeshActor.h"
#include "MeshDescription.h"

#include "DebugPlugin/DebugHelper.h"

#include "Kismet/GameplayStatics.h"


MeshExtractor::MeshExtractor()
{

}

MeshExtractor::~MeshExtractor()
{

}


//collect adges for an single actor passed per pointer
//data will get append to array
void MeshExtractor::ExtractMeshDataPerSection(AActorBase* actor, TArray<MeshData> &data){
    if(actor){
        TArray<UStaticMeshComponent *> componentsFound;
        actor->TFindAllChildsByType<UStaticMeshComponent>(componentsFound);
        ExtractMeshDataPerSection(componentsFound, data);
    }
}


void MeshExtractor::ExtractMeshDataPerSection(
    TArray<UStaticMeshComponent*> &components, 
    TArray<MeshData> &meshdata
){
    for (int i = 0; i < components.Num(); i++){
        if(UStaticMeshComponent *current = components[i]){
            ExtractMeshDataPerSection(current, meshdata);
        }
    }
}


void MeshExtractor::ExtractMeshDataPerSection(UStaticMeshComponent* comp, TArray<MeshData> &meshdata){
    if(comp){
        if(UStaticMesh *inner = comp->GetStaticMesh()){
            ExtractMeshDataPerSection(inner, meshdata);
        }
    }
}

void MeshExtractor::ExtractMeshDataPerSection(UStaticMesh *comp, TArray<MeshData> &meshdata){
    if(comp){
        const FStaticMeshRenderData* renderdata = comp->GetRenderData();
        if(renderdata){
            ExtractMeshDataPerSection(renderdata, meshdata, 0); //LOD 0 only.
        }
    }
}


void MeshExtractor::ExtractMeshDataPerSection(
    const FStaticMeshRenderData *renderdata, 
    TArray<MeshData> &meshdataArray,
    int32 LODIndex
){
    const FStaticMeshLODResources& LODResources = renderdata->LODResources[LODIndex];

    int numScetions = LODResources.Sections.Num();
    const FIndexArrayView Indices = LODResources.IndexBuffer.GetArrayView();
    const FPositionVertexBuffer& PositionVertexBuffer = LODResources.VertexBuffers.PositionVertexBuffer;

    int offset = meshdataArray.Num();
    int sizeBuffer = meshdataArray.Num() + numScetions;
    meshdataArray.SetNum(sizeBuffer);

    int index = 0;
    for (const FStaticMeshSection &Section : LODResources.Sections)
    {
        MeshData &current = meshdataArray[index];
        index++;

        int32 FirstIndex = Section.FirstIndex;
        int32 NumTris = Section.NumTriangles;

        for (int32 t = 0; t < NumTris; ++t)
        {
            int32 i = FirstIndex + t * 3;

            uint32 i0 = Indices[i + 0];
            uint32 i1 = Indices[i + 1];
            uint32 i2 = Indices[i + 2];

            // Triangle gehört zu Section.MaterialIndex

            FVector v0 = ToVectorDouble(PositionVertexBuffer.VertexPosition(i0));
            FVector v1 = ToVectorDouble(PositionVertexBuffer.VertexPosition(i1));
            FVector v2 = ToVectorDouble(PositionVertexBuffer.VertexPosition(i2));
            current.appendEfficent(v0, v1, v2);
        }
    }
}




//single meshdata


void MeshExtractor::ExtractSingleMeshData(AActorBase* actor, MeshData &meshdata){
    if(actor){
        TArray<UStaticMeshComponent *> componentsFound;
        actor->TFindAllChildsByType<UStaticMeshComponent>(componentsFound);
        ExtractSingleMeshData(componentsFound, meshdata);
    }
}


void MeshExtractor::ExtractSingleMeshData(
    TArray<UStaticMeshComponent*> &components, 
    MeshData &meshdata
){
    for (int i = 0; i < components.Num(); i++){
        if(UStaticMeshComponent *current = components[i]){
            ExtractSingleMeshData(current, meshdata);
        }
    }
}


void MeshExtractor::ExtractSingleMeshData(UStaticMeshComponent* comp, MeshData &meshData){
    if(comp){
        if(UStaticMesh *inner = comp->GetStaticMesh()){
            int32 lod = 0;
            ExtractSingleMeshData(inner, meshData, lod);
        }
    }
}

void MeshExtractor::ExtractSingleMeshData(
    UStaticMesh *staticmesh, 
    MeshData &singleMeshData,
    int32 LODIndex
){
    if(staticmesh){
        
        const FStaticMeshRenderData* renderdata = staticmesh->GetRenderData();
        if(renderdata){
            ExtractSingleMeshData(renderdata, singleMeshData, 0); //LOD 0 only.
        }
    
    }
}


void MeshExtractor::ExtractSingleMeshData(
    const FStaticMeshRenderData *renderdata, 
    MeshData &singleMeshData,
    int32 LODIndex
){
    if(renderdata){
        const FStaticMeshLODResources& LODResources = renderdata->LODResources[LODIndex];
        const FPositionVertexBuffer& PositionVertexBuffer = LODResources.VertexBuffers.PositionVertexBuffer;

        const FIndexArrayView Indices = LODResources.IndexBuffer.GetArrayView();

        for (int32 i = 0; i < Indices.Num(); i += 3)
        {
            uint32 i0 = Indices[i + 0];
            uint32 i1 = Indices[i + 1];
            uint32 i2 = Indices[i + 2];

            //local meshdata
            FVector v0 = ToVectorDouble(PositionVertexBuffer.VertexPosition(i0));
            FVector v1 = ToVectorDouble(PositionVertexBuffer.VertexPosition(i1));
            FVector v2 = ToVectorDouble(PositionVertexBuffer.VertexPosition(i2));

            singleMeshData.appendEfficent(v0, v1, v2);
        }   
    }
}

FVector MeshExtractor::ToVectorDouble(FVector3f vertex){
    return FVector(
        vertex.X,
        vertex.Y,
        vertex.Z
    );
}

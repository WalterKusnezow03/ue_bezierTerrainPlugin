// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreMinimal.h"
#include "p2/gamestart/assetEnums/materialEnum.h"
#include "p2/gamestart/assetManager.h"
#include "wingsuitMeshActor.h"



AwingsuitMeshActor::AwingsuitMeshActor(){

}
AwingsuitMeshActor::~AwingsuitMeshActor(){

}

void AwingsuitMeshActor::BeginPlay(){
    Super::BeginPlay();
    meshInited = false;
}


void AwingsuitMeshActor::Tick(float DeltaTime){
    Super::Tick(DeltaTime); //super class will also count animation time
    
}





void AwingsuitMeshActor::initWingsuitMesh(int detail){
    if(meshInited){
        return;
    }
    meshInited = true;

 
    /*
    sind die flügel eines dreiecks, a,b,c,d,e sollen getrennt manipulierbar sein,
    als index gespeichert

    a---b---c
            |
            d
            |
            e
    */

    //debugwise:
    FVector a(0, -200, 200);
    FVector b(0, -100, 200);
    FVector c(0, 0, 200);
    FVector d(0, 0, 100);
    FVector e(0, 0, 100);

    wingMeshDataLeft.setup(a, b, c, d, e, detail);
    wingMeshDataRight.setup(a, b, c, d, e, detail);
   

    //update mesh and apply material, for now layer 0!
    bool enableCollision = false;
    UProceduralMeshComponent *meshPointer = Mesh;
    if(meshPointer){
        updateMesh(
            *meshPointer,
            wingMeshDataLeft,
            layerLeft,
            enableCollision
        );
        updateMesh(
            *meshPointer,
            wingMeshDataRight,
            layerRight,
            enableCollision
        );

        assetManager *assetManagerInstance = assetManager::instance();
        if(assetManagerInstance != nullptr){
            UMaterialInterface *material = assetManagerInstance->findMaterial(
                materialEnum::wallMaterial
            );

            ApplyMaterial(
                meshPointer,
                material,
                layerLeft
            );
            ApplyMaterial(
                meshPointer,
                material,
                layerRight
            );
        }
        
    }


}




/**
 * 
 * ---- bone controller api ----
 * 
 */

void AwingsuitMeshActor::showMesh(bool open){
    if(meshInited){
        SetActorHiddenInGame(!open);
    }
}

void AwingsuitMeshActor::refreshVerteciesForBothWings(
    FVector &a,
    FVector &b,
    FVector &c,
    FVector &d,
    FVector &e,
    FVector &a1,
    FVector &b1,
    FVector &c1,
    FVector &d1,
    FVector &e1
){
    if(meshInited){
        refreshVertecies(a, b, c, d, e, wingMeshDataLeft);
        refreshVertecies(a1, b1, c1, d1, e1, wingMeshDataRight);

        if(Mesh){
            refreshMesh(
                *Mesh,
                wingMeshDataLeft,
                layerLeft
            );
            refreshMesh(
                *Mesh,
                wingMeshDataRight,
                layerRight
            );
        }
    }
}




void AwingsuitMeshActor::refreshVertecies(
    FVector &a,
    FVector &b,
    FVector &c,
    FVector &d,
    FVector &e,
    wingMeshData &meshdata
){
    if(meshInited){
        meshdata.refreshVertecies(a, b, c, d, e);
    }
}
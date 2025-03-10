// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "p2/meshgen/customMeshActorBase.h"
#include "p2/meshgen/MeshData.h"
#include "wingMeshData.h"
#include "wingsuitMeshActor.generated.h"

/**
 * 
 */
UCLASS()
class P2_API AwingsuitMeshActor : public AcustomMeshActorBase
{
	GENERATED_BODY()
	
public:
	AwingsuitMeshActor();
	~AwingsuitMeshActor();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void initWingsuitMesh(int detail);

	//API for tick
	void showMesh(bool open);
	void refreshVerteciesForBothWings(
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
	);

private:
	bool meshInited = false;

	class wingMeshData wingMeshDataLeft;
	class wingMeshData wingMeshDataRight;

	void refreshVertecies(
		FVector &a,
		FVector &b,
		FVector &c,
		FVector &d,
		FVector &e,
		wingMeshData &meshdata
	);

	const int layerLeft = 0;
    const int layerRight = 1;
};

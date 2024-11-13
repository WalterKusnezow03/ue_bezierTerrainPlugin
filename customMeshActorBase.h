// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include <map>
#include "customMeshActorBase.generated.h"

UCLASS()
class P2_API AcustomMeshActorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AcustomMeshActorBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//to be overriden by subclass
	virtual void createTerrainFrom2DMap(
		std::vector<std::vector<FVector>> &map,
		bool createTrees
	);

	static void createQuad(
		FVector &a,
		FVector &b,
		FVector &c,
		FVector &d,
		MeshData &output
	);

	static void createTwoSidedQuad(
		FVector &a,
		FVector &b,
		FVector &c,
		FVector &d,
		MeshData &output
	);

	void createTwoSidedQuad(
		FVector &a, 
		FVector &b, 
		FVector &c, 
		FVector &d, 
		UMaterial *material
	);

	void createTwoSidedQuad(
		FVector &a,
		FVector &b,
		FVector &c,
		FVector &d,
		UMaterial *material,
		bool calculateNormals
	);

protected:

	/// @brief saves the mesh data in a map for each layer, keeps things organized
	std::map<int, MeshData> meshLayersMap;

	UPROPERTY(VisibleAnywhere)
	class UProceduralMeshComponent *Mesh;

	
	void createTerrainFrom2DMap(
		std::vector<std::vector<FVector>> &map,
		bool createTrees,
		TArray<FVectorTouple> &touples
	);

	void updateMesh(MeshData otherMesh, bool createNormals, int layer);
	static void buildTriangle(
		FVector &a,
		FVector &b,
		FVector &c,
		TArray<FVector> &output,
		TArray<int32> &trianglesOutput
	);
	static void buildQuad(
		FVector &a,
		FVector &b,
		FVector &c,
		FVector &d,
		TArray<FVector> &output,
		TArray<int32> &trianglesOutput
	);






	//ok

	void ApplyMaterial(UProceduralMeshComponent *ProceduralMeshComponent, UMaterial *Material);
	void ApplyMaterial(
		UProceduralMeshComponent *ProceduralMeshComponent,
		UMaterial *Material,
		int layer
	);
};

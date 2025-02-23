// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "p2/util/FVectorTouple.h"
#include "ProceduralMeshComponent.h"
#include "p2/interfaces/Damageinterface.h"
#include "p2/player/teamEnum.h"
#include "Components/BoxComponent.h"
#include "p2/gamestart/assetEnums/materialEnum.h"
#include "p2/meshgen/MeshData.h"
#include "p2/meshgen/customMeshActorBase.h"
#include <map>
#include "customMeshActor.generated.h"

UCLASS()
class P2_API AcustomMeshActor : public AcustomMeshActorBase, public IDamageinterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AcustomMeshActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//derived methods
	//methods
	virtual void takedamage(int d) override;
	virtual void takedamage(int d, FVector &from) override;
	virtual void setTeam(teamEnum t) override;
	virtual teamEnum getTeam() override;

	//custom mesh actor methods

	//material behaviour to damage 
	void setMaterialBehaiviour(materialEnum mat);
	void setMaterialBehaiviour(materialEnum mat, bool split);
	void setMaterialBehaviourAndHealth(materialEnum mat, int health);
	void setMaterialAndHealthAndSplitOnDeath(materialEnum mat, int health, bool split);



	static void splitAndreplace(AActor *actor, FVector &bottom, int cmTile, materialEnum materialType);

	static void splitAndreplace(
		FVector &bottomCenter,
		int xBound,
		int yBound,
		int zBound,
		int cmTile,
		materialEnum materialType,
		UWorld *world
	);

	void createTerrainFrom2DMap(
		std::vector<std::vector<FVector>> &map,
		bool createTrees
	) override;

	void process2DMapSimple(
		std::vector<std::vector<FVector>> &map,
		MeshData &outputData
	);


	void createCube(
		FVector &a,
		FVector &b,
		FVector &c,
		FVector &d,
		FVector &dir,
		int cmheight,
		UMaterial *material
	);

	void createCube(
		FVector &a,
		FVector &b,
		FVector &c,
		FVector &d,
		FVector &dir,
		int cmheight,
		MeshData &outputMeshData
	);

	void createCube(
		FVector &a,
		FVector &b,
		FVector &c,
		FVector &d,
		FVector &a1,
		FVector &b1,
		FVector &c1,
		FVector &d1,
		UMaterial *material
	);

	void createCube(
		FVector &a,
		FVector &b,
		FVector &c,
		FVector &d,
		FVector &a1,
		FVector &b1,
		FVector &c1,
		FVector &d1,
		MeshData &meshDataOutput
	);


	void setDamagedOwner(IDamageinterface *damagedOwnerIn);

protected:
	
	int health = 100;
	bool destructableBool = false;
	bool splitOnDeath = false;
	bool isDestructable();
	void setHealth(int d);

	teamEnum team;

	class IDamageinterface *damagedOwner = nullptr;


	void createFoliage(TArray<FVectorTouple> &touples);
	void createTreeAndSaveMeshTo(
		FVector &location,
		MeshData &meshDataStem,
		MeshData &meshDataLeaf
	);

	materialEnum materialtypeSet = materialEnum::grassMaterial;
};

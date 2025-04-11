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
#include "p2/meshgen/MeshData/MeshData.h"
#include "p2/meshgen/foliage/ETerrainType.h"
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
	virtual void takedamage(int d, bool surpressed) override;
	virtual void takedamage(int d, FVector &hitpoint, bool surpressed) override;

	virtual void setTeam(teamEnum t) override;
	virtual teamEnum getTeam() override;

	//custom mesh actor methods

	//material behaviour to damage 
	void setMaterialBehaiviour(materialEnum mat);
	


	
	void splitIntoAllTriangles();
	void createNewMeshActors(std::vector<MeshData> &meshes, materialEnum material);


	void createTerrainFrom2DMap(
		std::vector<std::vector<FVector>> &map,
		bool createTrees,
		ETerrainType typeIn
	) override;

	


	void createCube(
		FVector &a,
		FVector &b,
		FVector &c,
		FVector &d,
		FVector &dir,
		int cmheight,
		materialEnum type
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
		materialEnum material
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
		MeshData &cubeMesh
	);

	void setDamagedOwner(IDamageinterface *damagedOwnerIn);

	void enableDebug();

protected:
	bool DEBUG_enabled = false;
	void debugThis(FVector &hitpoint);

	void createDebreeOnDamage(FVector &worldhit);

	int health = 100;
	bool destructableBool = false;
	bool splitOnDeath = false;
	bool isDestructable();
	void setHealth(int d);

	FVector worldToLocalHit(FVector &worldhit);

	teamEnum team;

	class IDamageinterface *damagedOwner = nullptr;


	void createFoliageAndPushNodesAroundFoliageToNavMesh(TArray<FVectorTouple> &touples);

	void createTreeAndSaveToMesh(FVector &location);

	materialEnum materialtypeSet = materialEnum::grassMaterial;


	//shader
	std::vector<materialEnum> foliageMaterials();

	void glassreactionToHitWorld(FVector &hitWorld);
	void glassreactionToHitLocal(FVector &hitlocal);
	void debugDrawMeshData(MeshData &meshdata);

	bool hasGlassMesh();
};

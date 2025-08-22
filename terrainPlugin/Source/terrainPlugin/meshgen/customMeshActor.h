// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameCore/util/FVectorTouple.h"
#include "ProceduralMeshComponent.h"

#include "Components/BoxComponent.h"
#include "AssetPlugin/gamestart/assetEnums/materialEnum.h"
#include "GameCore/MeshGenBase/MeshData/MeshData.h"
#include "GameCore/MeshGenBase/foliage/ETerrainType.h"
#include "GameCore/MeshGenBase/customMeshActorBase.h"
#include "terrainPlugin/meshgen/generation/helper/TerrainChunkSetup.h"
#include "terrainPlugin/meshgen/foliage/MatrixTree/MatrixTree.h"
#include <map>

#include "terrainPlugin/meshgen/generation/TerrainCreator/TerrainMeshDataParser/ChunkParser.h"


#include "customMeshActor.generated.h"


UCLASS()
class TERRAINPLUGIN_API AcustomMeshActor : public AcustomMeshActorBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AcustomMeshActor();

	static AcustomMeshActor *makeInstance(UWorld *world);

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

	//chunkParser Setup: will copy data, BUT KEEP POINTER, Parser must be a non value variable!
	//planned to be used as Ptr which is passed.
	//also applies actors location
	void UpdateMeshDataAndPosition(ChunkParser &parser);

	/// @brief releases the chunk parser pointer if possible and
	/// resets the usuage flag
	void releaseChunkParserPointer();

	


	//custom mesh actor methods

	//material behaviour to damage 
	void setMaterialBehaiviour(materialEnum mat);

	void addRandomNodesToNavmesh(TArray<FVectorTouple> &touples);

	void splitIntoAllTriangles();
	void createNewMeshActors(std::vector<MeshData> &meshes, materialEnum material);

	void createTerrainFrom2DMap(TerrainChunkSetup &package);

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

	
	

protected:

	//cunk parser reference for flag free
	ChunkParser *chunkParserPointer = nullptr;

	
	




	MatrixTree tree; //depracated to chunk parser


	void groundReactionToHitWorld(FVector &hitpoint);

	void createDebreeOnDamage(FVector &worldhit);


	bool destructableBool = false;
	bool splitOnDeath = false;
	virtual bool isDestructable();
	void setHealth(int d);




	void createFoliageAndPushNodesAroundFoliageToNavMesh(
		TArray<FVectorTouple> &touples,
		float percentDensity
	);

	void createTreeAndSaveToMesh(FVector &location);

	materialEnum materialtypeSet = materialEnum::grassMaterial;


	//shader
	std::vector<materialEnum> foliageMaterials();

	void glassreactionToHitWorld(FVector &hitWorld);
	void glassreactionToHitLocal(FVector &hitlocal);
	void debugDrawMeshData(MeshData &meshdata);

	bool hasGlassMesh();
};

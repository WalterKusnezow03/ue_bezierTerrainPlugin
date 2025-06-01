// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include <map>
#include "MeshDataLod.h"
#include "ELod.h"
#include "GameCore/util/FVectorTouple.h"
#include "GameCore/MeshGenBase/foliage/ETerrainType.h"
#include "customMeshActorBase.generated.h"

UCLASS()
class GAMECORE_API AcustomMeshActorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AcustomMeshActorBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool LISTEN_FOR_LOD_PLAYER = false;

public:
	void enableLodListening();
	void disableDistanceListening();

	void disablePhysicscollision();

	void enableCollisionOnLayer(materialEnum type, bool enable);
	void enableCollisionOnLayer(int layer, bool enable);
	void enableCollisionByPreset();

	ELod lodLevelByDistanceTo(FVector &other);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ApplyMaterial(
		materialEnum type
	);
	void ApplyMaterialNoRaycastLayer(
		materialEnum type
	);




protected:
	bool distanceListeningBlocked = false;

	void createTerrainFrom2DMap(
		std::vector<std::vector<FVector>> &map,
		ETerrainType typeIn
	);
	
	

public:

	void createTwoSidedQuad(
		FVector &a,
		FVector &b,
		FVector &c,
		FVector &d,
		materialEnum material
	);

	void replaceMeshData(MeshData &meshdata, materialEnum type, ELod lodLevel);
	void replaceMeshData(MeshData &meshdata, materialEnum type);

	void ReloadMeshAndApplyAllMaterials();
	void ReloadMeshForMaterial(materialEnum material);

	MeshData &findMeshDataReference(
		materialEnum type,
		ELod lodLevel,
		bool raycastOnLayer
	);

	MeshData &findMeshDataReference(
		materialEnum type,
		bool raycastOnLayer
	);


	

	void appendMeshDataAndReload(
		MeshData &meshdata,
		materialEnum type,
		ELod lodLevel,
		bool raycastOnLayer
	);


	//HIT REACT MATERIALS
	FVector worldToLocalHit(FVector &worldhit);
	bool doesHitWorld(FVector &hitWorld, materialEnum mat);
	bool doesHitLocal(FVector &hitLocal, materialEnum mat);

protected:
	int chunkScaleOneAxisLengthCm = 1; //ONE AXIS LENGTH

	
	void filterTouplesForVerticalVectors(
		TArray<FVectorTouple> &touples,
		std::vector<FVector> &potentialLocations
	);

	void appendLodTerrain(
		std::vector<std::vector<FVector>> &map,
		MeshData &grassLayer,
		MeshData &stoneLayer,
		int stepSize
	);

	ELod currentLodLevel = ELod::lodNear;
	void changeLodBasedOnPlayerPosition();

	

	/// @brief saves the mesh data in a map for each layer, keeps things organized
	// std::map<int, MeshData> meshLayersMap;
	// std::map<int, MeshData> meshLayersMapNoRaycast;

	UPROPERTY(VisibleAnywhere)
	class UProceduralMeshComponent *Mesh;

	UPROPERTY(VisibleAnywhere)
	class UProceduralMeshComponent *MeshNoRaycast;

	/// @brief saves mesh data per lod level per material int
	std::map<int, MeshDataLod> meshLayersLodMap;
	std::map<int, MeshDataLod> meshLayersLodMapNoRaycast;

	std::map<ELod, UProceduralMeshComponent *> meshComponentLodMap;
	std::map<ELod, UProceduralMeshComponent *> meshComponentLodMapNoRaycast;
	
	UProceduralMeshComponent *MeshForLod(ELod lod);
	UProceduralMeshComponent *MeshNoRaycastForLod(ELod lod);
	void switchToLod(ELod lod);
	void initLodMeshesOnBeginPlay();

	void ReloadMeshForMaterialByLod(ELod lod, materialEnum material);
	

	//allows custom mesh without Lod
	void updateMesh(
		UProceduralMeshComponent &meshcomponent,
		MeshData &otherMesh,
		int layer,
		bool enableCollision
	);

	



	


	void ApplyMaterial(UProceduralMeshComponent *ProceduralMeshComponent, UMaterialInterface *Material);
	void ApplyMaterial(
		UProceduralMeshComponent *ProceduralMeshComponent,
		UMaterialInterface *Material,
		int layer
	);
	void ApplyMaterial(
		UProceduralMeshComponent *ProceduralMeshComponent,
		materialEnum type
	);

	bool isInRange(FVector &a, int maxDistance, int &smallestEdge);

	ETerrainType thisTerrainType = ETerrainType::ETropical;

	///new: refacture for moving meshes already in base actor
	virtual void refreshMesh(
		UProceduralMeshComponent &meshComponent,
		MeshData &other,
		int layer
	);
	float shaderRunningTime = 0.0f;
	void TickShaderRunningTime(float time);
	void vertexShaderFor(MeshData &data);
	virtual void applyShaderToVertex(FVector &vertex);

	int maxDistanceForLod(ELod lodLevel);


public:
	static int layerByMaterialEnum(materialEnum type);
	static std::vector<materialEnum> materialVector();
	static std::vector<ELod> lodVector();
	static std::vector<ETerrainType> terrainVector();

	static materialEnum groundMaterialFor(ETerrainType terraintype);



};

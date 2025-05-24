// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameCore/MeshGenBase/customMeshActorBase.h"
#include "GameCore/interfaces/DamageInterface.h"
#include "ripple.h"
#include "customWaterActor.generated.h"

/**
 * 
 */
UCLASS()
class TERRAINPLUGIN_API AcustomWaterActor : public AcustomMeshActorBase, public IDamageinterface
{
	GENERATED_BODY()
	
public:
	AcustomWaterActor();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void takedamage(int d) override;
	virtual void takedamage(int d, FVector &from) override;
	virtual void takedamage(int d, bool surpressed) override;
	virtual void takedamage(int d, FVector &hitpoint, bool surpressed) override;

	virtual void setTeam(teamEnum t) override;
	virtual teamEnum getTeam() override;

	void createWaterPane(int vertexcountXIn, int detail);

	static void createWaterPane(
		UWorld *world,
		FVector location,
		int scaleMeters
	);

protected:
	ELod latestLodMeasured = ELod::lodNear;

	bool playerIsInRenderRange();//new

	bool inBoundsOfPane(FVector &vec);

	

	static const int MAX_VERTEXCOUNT = 50;
	static const int DEFAULT_DISTANCE_BETWEEN_VERTECIES = 100;
	int MAX_DISTANCE = 10000; // 10000; // 100000;

	teamEnum teamSaved = teamEnum::none;

	int vertexcountX = 3;
	int vertexcountY = 3;

	FVector BottomLeft;
	FVector BottomRight;
	FVector TopRight;
	FVector TopLeft;

	//axis locking for edge chunks
	bool topAxisLocked = false;
	bool bottomAxisLocked = false;
	bool leftAxisLocked = false;
	bool rightAxisLocked = false;
	void unlockAllAxis();
	bool isAtLockedAxis(FVector &other);
	void lockOuterAxisBasedOn(
		FVector &playerLocation,
		FVector &playerLookDir
	);

	//only update if mesh was inited
	virtual void refreshMesh(
		UProceduralMeshComponent &meshComponent,
		MeshData &other,
		int layer
	) override;

	void updateRunningTime(float deltaTime);
	void vertexShader();
	virtual void applyShaderToVertex(FVector &vertex) override;
	void resetAllShaderOffsets();
	void resetVertexShadignFor(FVector &other);

	
	bool playerIsInBounds();

	bool meshInited = false;
	

	
	//ripple section
	float ownHalfSize = 100.0f;

	int rippleVecSize = 0;
	std::vector<ripple> rippleVector;
	void TickRipples(float DeltaTime);
	void applyWaterRippleOffset(FVector &vertex, FVector &actorLocation);
	void addNewRipple(FVector &location);
	bool rippleIndexIsValid(int index);
	void removeRippleAtIndex(int index);


	//helper for mesh
	MeshData &findMeshDataReference(
		materialEnum mat,
		ELod lod
	);
	UProceduralMeshComponent *meshComponentPointer();


};

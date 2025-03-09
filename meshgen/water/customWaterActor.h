// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "p2/meshgen/customMeshActorBase.h"
#include "ripple.h"
#include "customWaterActor.generated.h"

/**
 * 
 */
UCLASS()
class P2_API AcustomWaterActor : public AcustomMeshActorBase, public IDamageinterface
{
	GENERATED_BODY()
	
public:
	AcustomWaterActor();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void takedamage(int d) override;
	virtual void takedamage(int d, FVector &from) override;
	virtual void setTeam(teamEnum t) override;
	virtual teamEnum getTeam() override;

	void createWaterPane(int vertexcountXIn, int detail);

	static void createWaterPane(
		UWorld *world,
		int vertexCount
	);

protected:
	bool inBoundsOfPane(FVector &vec);	
	bool doTick();

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

	//only update if mesh was inited
	virtual void refreshMesh(
		UProceduralMeshComponent &meshComponent,
		MeshData &other,
		int layer) override;

	void updateRunningTime(float deltaTime);
	void vertexShader();
	virtual void applyShaderToVertex(FVector &vertex) override;

	FVector playerLocation();
	bool playerIsInBounds();

	bool meshInited = false;
	float runningTime = 0.0f;

	
	//ripple section
	int rippleVecSize = 0;
	std::vector<ripple> rippleVector;
	void TickRipples(float DeltaTime);
	void applyWaterRippleOffset(FVector &vertex, FVector &actorLocation);
	void addNewRipple(FVector &location);
	bool rippleIndexIsValid(int index);
	void removeRippleAtIndex(int index);

	bool TickBasedOnPlayerDistance();

	bool isInRangeForTick(FVector &vertex, FVector &comparePos);

	//helper for mesh
	MeshData &findMeshDataReference(
		materialEnum mat,
		ELod lod
	);
	UProceduralMeshComponent *meshComponentPointer();





	int isInRangeForTickOnX(
		FVector &vertex,
		FVector &locationOfPlayer);
	int isInRangeForTickOnY(
		FVector &vertex,
		FVector &locationOfPlayer);


};

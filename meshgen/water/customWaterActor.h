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

	void createWaterPane(int vertexcountXIn, int vertexcountYIn, int detail);

private:
	teamEnum teamSaved = teamEnum::none;

	int vertexcountX = 3;
	int vertexcountY = 3;

	void refreshMesh(
		UProceduralMeshComponent &meshComponent,
		MeshData &other,
		int layer
	);

	void updateRunningTime(float deltaTime);
	void vertexShader();
	void applyCurve(FVector &vertex);

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

	//helper for mesh
	MeshData &findMeshDataReference(
		materialEnum mat,
		ELod lod
	);
	UProceduralMeshComponent *meshComponentPointer();
};

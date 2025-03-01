// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "p2/meshgen/customMeshActorBase.h"
#include "customWaterActor.generated.h"

/**
 * 
 */
UCLASS()
class P2_API AcustomWaterActor : public AcustomMeshActorBase
{
	GENERATED_BODY()
	
public:
	AcustomWaterActor();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	void createWaterPane(int vertexcountXIn, int vertexcountYIn, int detail);

private:
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
};

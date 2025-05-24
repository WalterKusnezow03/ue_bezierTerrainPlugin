// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameCore/util/TVector.h"


/**
 * 
 */
class TERRAINPLUGIN_API bezierCurve
{
public:
	bezierCurve();
	~bezierCurve();

	void calculatecurve(
		std::vector<FVector2D> &ref,
		TVector<FVector2D> &output,
		float _einheitsValue
	);

	void calculatecurve(
		std::vector<FVector> &ref, 
		std::vector<FVector> &output,
		float _einheitsValue
	);


	void createNewRandomCurve(
		FVector2D &startingPoint,
		TVector<FVector2D> &output,
		float _einheitsValue,
		float distanceBetweenAnchorsOnXAxisMin,
    	float distanceBetweenAnchorsOnXAxisMax,
		float distanceBetweenAnchorsYRange,
		float max_xy_coordinate
	);

	void createNewRandomCurve(
		FVector2D &startingPoint,
		TVector<FVector2D> &output,
		float _einheitsValue,
		float distanceMin,
		float distanceMax,
		float angleTurnMinAbs,
		float angleTurnMaxAbs,
		float max_xy_coordinate
	);

	void afterSmoothHeight(
		TArray<FVector> &curve,
		float _einheitsValue,
		int anchorSkipPerStep
	);

private:

	float EinheitsValue;


	void createContinuityCurve(std::vector<FVector2D> &anchors);
	
	void smoothAnchors(std::vector<FVector2D> &ref);
	
	void processAllPoints(std::vector<FVector2D> &points, TVector<FVector2D> &output);
	void process4Points(std::vector<FVector2D> &points, int offset, TVector<FVector2D> &output);
	

	FVector2D FVector2DFourAnchorBezier(
		FVector2D &a,
		FVector2D &b,
		FVector2D &c,
		FVector2D &d,
		float skalar
	);

	float validatePercentage(float val);

	void createNewtangentsAndPushP2P3P4(
		FVector2D &p0,
		FVector2D &p3,
		std::vector<FVector2D> &curve,
		float constSkalar
	);


	//random anchors validation
	FVector2D randomOffset2D(
		float xMin,
		float xMax,
		float yMin,
		float yMax
	);
	FVector2D NewAnchorValidated(
		FVector2D &latestAnchor, 
		FVector2D &offset, 
		float maxYValue
	);

	//rotation anchors validation
	bool inBoundY(FVector2D &other, float min, float max);
	FVector2D createNewValidAnchor(
		FVector2D &anchorPrev,
		FVector2D &direction,
		MMatrix &integratedRotation,
		float minAngle,
		float maxAngle,
		float maxXY
	);


};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class P2_API bezierCurve
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
};

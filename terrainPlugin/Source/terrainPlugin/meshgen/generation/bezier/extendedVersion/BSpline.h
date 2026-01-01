// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "terrainPlugin/meshgen/generation/bezier/extendedVersion/TBSplinePosition.h"

/**
 * b-Spline generator.
 */
class TERRAINPLUGIN_API BSpline
{
public:
	BSpline();
	~BSpline();

    // --- 2d spline ---
	void calculatecurve(
		const TArray<FVector2D> &ref,
		TArray<FVector2DBSplinePosition> &output,
		float _einheitsValue
	);
    

    // --- 3d spline ---
    void calculatecurve(
        const TArray<FVector> &ref,
        TArray<FVectorBSplinePosition> &output,
        float _einheitsValue
    );
    

    void afterSmoothHeight(
		TArray<FVectorBSplinePosition> &curve,
		float _einheitsValue,
		int anchorSkipPerStep
	);

private:

    void calculatecurve(
        TArray<FVectorBSplinePosition> &ref,
        TArray<FVectorBSplinePosition> &output,
        float _einheitsValue
    );

    void calculatecurve(
		TArray<FVector2DBSplinePosition> &ref,
		TArray<FVector2DBSplinePosition> &output,
		float _einheitsValue
	);

	
	float EinheitsValue;


	void createContinuityCurve(TArray<FVector2DBSplinePosition> &anchors);
	
	void smoothAnchors(TArray<FVector2DBSplinePosition> &ref);
	
	void processAllPoints(
        TArray<FVector2DBSplinePosition> &points, 
        TArray<FVector2DBSplinePosition> &output
    );
	void process4Points(
        TArray<FVector2DBSplinePosition> &points, 
        int offset, 
        TArray<FVector2DBSplinePosition> &output
    );
	

	FVector2DBSplinePosition FVector2DFourAnchorBezier(
		FVector2DBSplinePosition &a,
		FVector2DBSplinePosition &b,
		FVector2DBSplinePosition &c,
		FVector2DBSplinePosition &d,
		float skalar
	);
    FVector2DBSplinePosition FVector2DFourAnchorBezier(
        FVector2D &a,
        FVector2D &b,
        FVector2D &c,
        FVector2D &d,
        float skalar
    );

    float validatePercentage(float val);

	void createNewtangentsAndPushP2P3P4(
		FVector2DBSplinePosition &p0,
		FVector2DBSplinePosition &p3,
		TArray<FVector2DBSplinePosition> &curve,
		float constSkalar
	);



};

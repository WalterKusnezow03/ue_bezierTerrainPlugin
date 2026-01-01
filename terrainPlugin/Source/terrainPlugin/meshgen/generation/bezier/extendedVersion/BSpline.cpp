// Fill out your copyright notice in the Description page of Project Settings.


#include "BSpline.h"
#include "GameCore/util/FVectorUtil.h"
#include "CoreMath/Matrix/MMatrix.h"

#include "DebugPlugin/DebugHelper.h"

BSpline::BSpline()
{
    EinheitsValue = 100;
}

BSpline::~BSpline()
{
}



void BSpline::calculatecurve(
    const TArray<FVector2D> &ref,
    TArray<FVector2DBSplinePosition> &output,
    float _einheitsValue
){
    TArray<FVector2DBSplinePosition> converted;
    for (int i = 0; i < ref.Num(); i++){
        converted.Add(
            FVector2DBSplinePosition(ref[i], i) // is marked as original index by default
        );
    }
    calculatecurve(converted, output, _einheitsValue);
}

/// @brief will calculate the bezier tangential spline for you, EXPECTS ANCHORS TO BE ALONG X AXIS!
/// @param ref reference anchor points (p0s and p3s)
/// @param output output vector to save in, MUST BE CLEAR
/// @param _einheitsValue einheits value (distance step) between vectors, for example 100cm unreal engine scale
void BSpline::calculatecurve(
    TArray<FVector2DBSplinePosition> &ref, 
    TArray<FVector2DBSplinePosition> &output,
    float _einheitsValue
){
    //cant create a curve if has less than 3 anchors
    if(ref.Num() < 3){
        return;
    }

    //ISSUE NOT ALLOWED DIVISION BY ZERO
    EinheitsValue = std::abs(_einheitsValue); //positive values only
    if(EinheitsValue == 0){
        return;
    }

    //debug print
    FString message = TEXT("BSpline::calculatecurve 2D ");
    for(FVector2DBSplinePosition &in : ref){
        message +=
            FString::Printf(
                TEXT("(%.2f %.2f)"),
                in.position.X,
                in.position.Y
            );
    }
    DebugHelper::logMessage(message);

    //create and add curve anchors
    createContinuityCurve(ref);


    //interpolate
    processAllPoints(ref, output); //what if you do it more than once
   
}


/**
 * 
 * ---- new 3D vertecies ---- 
 * 
 */
void BSpline::calculatecurve(
    const TArray<FVector> &ref,
    TArray<FVectorBSplinePosition> &output,
    float _einheitsValue
){
    TArray<FVectorBSplinePosition> converted;
    for (int i = 0; i < ref.Num(); i++){
        converted.Add(
            FVectorBSplinePosition(ref[i], i) // is marked as original index by default
        );
    }
    calculatecurve(converted, output, _einheitsValue);
}



void BSpline::calculatecurve(
    TArray<FVectorBSplinePosition> &ref, 
    TArray<FVectorBSplinePosition> &output,
    float _einheitsValue
){
    //if x doesnt change, no segments are generated, prevent by switching axis temporarly
    bool switchedX = false;
    for (int i = 1; i < ref.Num(); i++){
        FVector &prev = ref[i - 1].GetPosition();
        FVector &current = ref[i].GetPosition();
        if (prev.X != current.X)
        {
            switchedX = true;
        }
    }

    /// --- generation ---
    TArray<FVector2DBSplinePosition> vecA;
    TArray<FVector2DBSplinePosition> vecB;
    for (int i = 0; i < ref.Num(); i++){
        FVectorBSplinePosition &current = ref[i];
        FVector currentPos = current.GetPosition();

        //x y switch to prevent degenerate segments
        if(!switchedX){
            float copy = currentPos.Y;
            currentPos.Y = currentPos.X;
            currentPos.X = copy;
        }

        //marked as dafault anchor automatically
        vecA.Add(FVector2DBSplinePosition(currentPos.X, currentPos.Y, i));
        vecB.Add(FVector2DBSplinePosition(currentPos.X, currentPos.Z, i));
    }

    TArray<FVector2DBSplinePosition> outA;
    TArray<FVector2DBSplinePosition> outB;
    calculatecurve(
        vecA,
        outA,
        _einheitsValue
    );
    calculatecurve(
        vecB,
        outB,
        _einheitsValue
    );

    DebugHelper::logMessage(
        FString::Printf(
            TEXT(
                "BSpline::CurveA %d CurveB %d from VecA %d VecB %d "
            ),
            outA.Num(),
            outB.Num(),
            vecA.Num(),
            vecB.Num()
        )
    );

    //merge
    int larger = outA.Num() > outB.Num() ? outA.Num() : outB.Num();
    for (int i = 0; i < larger; i++)
    {
        FVector2DBSplinePosition currentA;
        FVector2DBSplinePosition currentB;
        if(i < outA.Num()){
            currentA = outA[i];
        }else{
            currentA = outA[outA.Num() - 1];
        }

        if(i < outB.Num()){
            currentB = outB[i];
        }else{
            currentB = outB[outB.Num() - 1];
        }

        if(switchedX){
            currentA.flip();
            currentB.flip();
        }

        FVectorBSplinePosition merge(currentA, currentB);
        output.Add(merge);
    }
}

/// @brief iterates over the anchors and moves them closer on y axis together based on their distance on x
/// @param ref to smooth out
void BSpline::smoothAnchors(TArray<FVector2DBSplinePosition> &ref){
    float decreaseFrac = 0.1f; //percent
    for (int i = 1; i < ref.Num(); i++)
    {
        FVector2D &a = ref[i-1].GetPosition();
        FVector2D &b = ref[i].GetPosition();
        float distX = std::abs(a.X - b.X);
        float distY = std::abs(a.Y - b.Y);
        if(distX / 2 < distY){
            if(a.Y < b.Y){
                b.Y = std::max(b.Y - distY * decreaseFrac, a.Y); //max to not go too small
            }
            if(a.Y > b.Y){
                b.Y = std::min(b.Y + distY * decreaseFrac, a.Y); //min to not go too big
            }
        }
    }
}

///@brief calculates the tangential anchors and adds them to anchors in correct order
/// will override the data and create the points including tangential extensions in correct order
void BSpline::createContinuityCurve(TArray<FVector2DBSplinePosition> &anchors){
    if(anchors.Num() < 2){
        return;
    }


    //data will be copied later
    TArray<FVector2DBSplinePosition> curve;

    float BETAConst = 0.25;

    int i = 1; 
    while(i < anchors.Num()){

        if(i == 1){
            //first curve part
            FVector2DBSplinePosition &p0 = anchors[i-1]; //start
            FVector2DBSplinePosition &p3 = anchors[i];  // at(i); //end

           
            FVector2D direction = p3.GetPosition() - p0.GetPosition();
            direction += FVector2D(0, direction.Y * 2); // randomness for first point

            //is just linear at first
            FVector2DBSplinePosition p1(p0.GetPosition() + (direction * BETAConst)); 

            curve.Add(p0);
            curve.Add(p1);

            createNewtangentsAndPushP2P3P4(
                p0,
                p3,
                curve,
                BETAConst
            );
        
            i++;
        }else{
            
            FVector2DBSplinePosition &p3 = curve[curve.Num() - 2]; //prev of p4 is p3

            
            if(i < anchors.Num() - 1){
                //default interpolation:

                //p3 und p4 wurden dann ja von vorher gepusht!, nurnoch to next tangent, point und next next tangent
                FVector2DBSplinePosition &p6 = anchors[i]; //next anchor
                createNewtangentsAndPushP2P3P4(
                    p3,
                    p6,
                    curve,
                    BETAConst
                );

                /*
                FVector2D dirToNextAnchor = (p6 - p3);

                //create instead p5 and push p6
                FVector2D tangentToNext_p5 = p6 - dirToNextAnchor * BETAConst; //tangent to p6 from p3
                curve.push_back(tangentToNext_p5);
                curve.push_back(p6);

                FVector2D tangentFromNext_p7 = p6 + dirToNextAnchor * BETAConst;
                curve.push_back(tangentFromNext_p7); //is p4 for next!
                */
            }
            else
            {
                //last interpolation
                FVector2DBSplinePosition &p3_current = p3;
                FVector2DBSplinePosition &finalPoint = anchors[anchors.Num() - 1];
                FVector2D dirToNextAnchor(finalPoint.GetPosition() - p3_current.GetPosition());
                FVector2DBSplinePosition tangentToFinal(finalPoint.GetPosition() - (dirToNextAnchor * BETAConst));

                //p0 and p1 already in list
                curve.Add(tangentToFinal);
                curve.Add(finalPoint);

                
            }

            i++;
        }

    }

    //copy to anchors, i defined before
    i = 0;
    for (i = 0; i < curve.Num(); i++)
    {
        if(i < anchors.Num()){
            anchors[i] = curve[i]; //copy
        }
        else{
            anchors.Add(curve[i]);
        }
    }


    //debug - might be dropped. - here ok
    int count = 0;
    for (i = 0; i < anchors.Num(); i++){
        if(anchors[i].IsOriginalAnchor()){
            count++;
        }
    }
    DebugHelper::logMessage("BSpline::Anchors Kept", count);
}

/// @brief new tangents will be created (pushed into vector: p2, p3, p4)
/// @param p0 
/// @param p3 
/// @param curve 
/// @param constSkalar 
void BSpline::createNewtangentsAndPushP2P3P4(
    FVector2DBSplinePosition &p0,
    FVector2DBSplinePosition &p3,
    TArray<FVector2DBSplinePosition> &curve,
    float constSkalar
){
    FVector2D dirToNextAnchor = p3.GetPosition() - p0.GetPosition();

    FVector2D tangentToNext_p2 = p3.GetPosition() - (dirToNextAnchor * constSkalar); //tangent to p6 from p3
    curve.Add(FVector2DBSplinePosition(tangentToNext_p2));
    curve.Add(p3);

    FVector2D tangentFromNext_p4 = p3.GetPosition() + (dirToNextAnchor * constSkalar);
    curve.Add(FVector2DBSplinePosition(tangentFromNext_p4)); //is p4 for next! (p1)
}









/// @brief 
/// @param points anchors to create the curve from, points will be overriden 
/// @param reserve space to be resevered (which was also reserved for points vector)
void BSpline::processAllPoints(
    TArray<FVector2DBSplinePosition> &points,
    TArray<FVector2DBSplinePosition> &output
){

    //the curves overlap like this
    //index:      [0] [1] [2] [3] [4] //which makes i += 3 (is correct.)
    //first part: p0, p1, p2, p3, p4
    //second part:            p0, p1, p2, p3
    //                               [5], [6]

    int next = 3;
    int size = points.Num() - 3;
    for (int i = 0; i < size; i += next)
    {
        process4Points(points, i, output);
    }
}

/// @brief will process the cubic bezier part
/// @param points points to process from
/// @param offset offset, index must be p0
/// @param output output to save in
void BSpline::process4Points(
    TArray<FVector2DBSplinePosition> &points, 
    int offset, 
    TArray<FVector2DBSplinePosition> &output
){
    if(offset + 3 >= points.Num()){
        return;
    }

    //std::vector<FVector2D> tmpListCurve;

    FVector2DBSplinePosition &p0 = points[offset];
    FVector2DBSplinePosition &p1 = points[offset + 1];
    FVector2DBSplinePosition &p2 = points[offset + 2];
    FVector2DBSplinePosition &p3 = points[offset + 3];

    //umwandeln zu inkrementell
    float distanceX = std::abs(p3.GetPosition().X - p0.GetPosition().X);

    for (float i = 0; i < distanceX; i += EinheitsValue)
    {

        float skalar = i / distanceX; //x / 1 full
        FVector2DBSplinePosition newPos = FVector2DFourAnchorBezier(p0, p1, p2, p3, skalar);

        //keep anchor
        if(i == 0){
            newPos.CopyIndexAndAnchorFlag(p0);
        }
        if(i == distanceX -1){
            newPos.CopyIndexAndAnchorFlag(p3);
        }

        output.Add(newPos);
    }
}





FVector2DBSplinePosition BSpline::FVector2DFourAnchorBezier(
    FVector2DBSplinePosition &a,
    FVector2DBSplinePosition &b,
    FVector2DBSplinePosition &c,
    FVector2DBSplinePosition &d,
    float skalar
){
    return FVector2DFourAnchorBezier(
        a.GetPosition(),
        b.GetPosition(),
        c.GetPosition(),
        d.GetPosition(),
        skalar
    );
}

FVector2DBSplinePosition BSpline::FVector2DFourAnchorBezier(
    FVector2D &a,
    FVector2D &b,
    FVector2D &c,
    FVector2D &d,
    float skalar
){
    FVector2D ab = a + skalar * (b - a);
    FVector2D bc = b + skalar * (c - b);
    FVector2D cd = c + skalar * (d - c);
    FVector2D abbc = ab + skalar * (bc - ab); //das ding dazwischen
    FVector2D bccd = bc + skalar * (cd - bc); //das ding dazwischen
    FVector2D abbcbccd = abbc + skalar * (bccd - abbc); //das ding dazwischen
    return FVector2DBSplinePosition(abbcbccd);    
}









/*

------ after smooth ------

testing needed !
*/
void BSpline::afterSmoothHeight(
    TArray<FVectorBSplinePosition> &curve,
    float _einheitsValue,
    int anchorSkipPerStep
){
    anchorSkipPerStep = std::max(std::abs(anchorSkipPerStep), 1);

    //copy anchors
    TArray<FVectorBSplinePosition> anchors;
    for(int i = 0; i < curve.Num(); i += anchorSkipPerStep){
        anchors.Add(curve[i]);

        int nextIndex = i + anchorSkipPerStep;
        if(nextIndex >= curve.Num()){
            anchors.Add(curve[curve.Num() - 1]);
        }
    }


    //calculate
    TArray<FVectorBSplinePosition> output;
    calculatecurve(
        anchors, 
        output,
        _einheitsValue
    );


    //apply smooth 
    for(int i = 0; i < output.Num(); i++){
        if(i < curve.Num()){
            FVector &interpolated = output[i].GetPosition();
            FVector &current = curve[i].GetPosition();
            current.Z = interpolated.Z;
        }
    }


}
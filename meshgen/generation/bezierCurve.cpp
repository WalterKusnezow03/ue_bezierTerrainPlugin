// Fill out your copyright notice in the Description page of Project Settings.


#include "bezierCurve.h"

bezierCurve::bezierCurve()
{
    EinheitsValue = 100;
}

bezierCurve::~bezierCurve()
{
}

/// @brief will calculate the bezier tangential spline for you, EXPECTS ANCHORS TO BE ALONG X AXIS!
/// @param ref reference anchor points (p0s and p3s)
/// @param output output vector to save in, MUST BE CLEAR
/// @param _einheitsValue einheits value (distance step) between vectors, for example 100cm unreal engine scale
void bezierCurve::calculatecurve(
    std::vector<FVector2D> &ref, 
    TVector<FVector2D> &output,
    float _einheitsValue
){
    //cant create a curve if has less than 3 anchors
    if(ref.size() < 3){
        return;
    }

    //ISSUE NOT ALLOWED DIVISION BY ZERO
    EinheitsValue = std::abs(_einheitsValue); //positive values only
    if(EinheitsValue == 0){
        return;
    }

    
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
void bezierCurve::calculatecurve(
    std::vector<FVector> &ref, 
    std::vector<FVector> &output,
    float _einheitsValue
){
    std::vector<FVector2D> vecA;
    std::vector<FVector2D> vecB;
    for (int i = 0; i < ref.size(); i++){
        FVector &current = ref[i];
        vecA.push_back(FVector2D(current.X, current.Y));
        vecB.push_back(FVector2D(current.X, current.Z));
    }

    TVector<FVector2D> outA;
    TVector<FVector2D> outB;
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

    //merge
    int larger = outA.size() > outB.size() ? outA.size() : outB.size();
    for (int i = 0; i < larger; i++)
    {
        FVector2D currentA;
        FVector2D currentB;
        if(i < outA.size()){
            currentA = outA[i];
        }else{
            currentA = outA[outA.size() - 1];
        }

        if(i < outB.size()){
            currentB = outB[i];
        }else{
            currentB = outB[outB.size() - 1];
        }

        FVector merge(currentA.X, currentA.Y, currentB.Y);
        output.push_back(merge);
    }
}

/// @brief iterates over the anchors and moves them closer on y axis together based on their distance on x
/// @param ref to smooth out
void bezierCurve::smoothAnchors(std::vector<FVector2D> &ref){
    float decreaseFrac = 0.1f; //percent
    for (int i = 1; i < ref.size(); i++)
    {
        FVector2D &a = ref[i-1];
        FVector2D &b = ref[i];
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
void bezierCurve::createContinuityCurve(std::vector<FVector2D> &anchors){
    if(anchors.size() < 2){
        return;
    }


    //data will be copied later
    std::vector<FVector2D> curve;

    float BETAConst = 0.25;

    int i = 1; 
    while(i < anchors.size()){

        if(i == 1){
            //first curve part
            FVector2D p0 = anchors.at(i-1); //start
            FVector2D p3 = anchors.at(i);  // at(i); //end

            FVector2D direction = (p3 - p0);
            direction += FVector2D(0, direction.Y * 2); //randomness for first point

            //is just linear at first
            FVector2D p1 = p0 + direction * BETAConst; 
            //FVector2D p2 = p3 - direction * BETAConst; 

            //FVector2D p4 = p3 + direction * BETAConst;

            curve.push_back(p0);
            curve.push_back(p1);

            createNewtangentsAndPushP2P3P4(
                p0,
                p3,
                curve,
                BETAConst
            );
            //curve.push_back(p2);
            //curve.push_back(p3);
            //curve.push_back(p4); //p1 für den nächsten ist das. 


            i++;
        }else{
            
            FVector2D p3 = curve.at(curve.size() - 2); //prev of p4 is p3

            
            if(i < anchors.size() - 1){
                //default interpolation:

                //p3 und p4 wurden dann ja von vorher gepusht!, nurnoch to next tangent, point und next next tangent
                FVector2D p6 = anchors.at(i); //next anchor
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
                FVector2D p3_current = p3;
                FVector2D finalPoint = anchors.at(anchors.size() - 1);
                FVector2D dirToNextAnchor = (finalPoint - p3_current);
                FVector2D tangentToFinal = finalPoint - dirToNextAnchor * BETAConst;

                //p0 and p1 already in list
                curve.push_back(tangentToFinal);
                curve.push_back(finalPoint);

                
            }

            i++;
        }

    }

    //copy to anchors, i defined before
    i = 0;
    for (i = 0; i < curve.size(); i++)
    {
        if(i < anchors.size()){
            anchors.at(i) = curve.at(i); //copy
        }
        else{
            anchors.push_back(curve.at(i));
        }
    }

}



/// @brief new tangents will be created (pushed into vector: p2, p3, p4)
/// @param p0 
/// @param p3 
/// @param curve 
/// @param constSkalar 
void bezierCurve::createNewtangentsAndPushP2P3P4(
    FVector2D &p0,
    FVector2D &p3,
    std::vector<FVector2D> &curve,
    float constSkalar
){
    FVector2D dirToNextAnchor = (p3 - p0);

    FVector2D tangentToNext_p2 = p3 - dirToNextAnchor * constSkalar; //tangent to p6 from p3
    curve.push_back(tangentToNext_p2);
    curve.push_back(p3);

    FVector2D tangentFromNext_p4 = p3 + dirToNextAnchor * constSkalar;
    curve.push_back(tangentFromNext_p4); //is p4 for next! (p1)
}









/// @brief 
/// @param points anchors to create the curve from, points will be overriden 
/// @param reserve space to be resevered (which was also reserved for points vector)
void bezierCurve::processAllPoints(
    std::vector<FVector2D> &points,
    TVector<FVector2D> &output
){

    //the curves overlap like this
    //index:      [0] [1] [2] [3] [4] //which makes i += 3 (is correct.)
    //first part: p0, p1, p2, p3, p4
    //second part:            p0, p1, p2, p3
    //                               [5], [6]

    int next = 3;
    int size = points.size() - 3;
    for (int i = 0; i < size; i += next)
    {
        process4Points(points, i, output);
    }
}

/// @brief will process the cubic bezier part
/// @param points points to process from
/// @param offset offset, index must be p0
/// @param output output to save in
void bezierCurve::process4Points(
    std::vector<FVector2D> &points, 
    int offset, 
    TVector<FVector2D> &output
){
    if(offset + 3 >= points.size()){
        return;
    }

    //std::vector<FVector2D> tmpListCurve;

    FVector2D &p0 = points[offset];
    FVector2D &p1 = points[offset + 1];
    FVector2D &p2 = points[offset + 2];
    FVector2D &p3 = points[offset + 3];

    //umwandeln zu inkrementell
    float distanceX = std::abs(p3.X - p0.X);

    for (float i = 0; i < distanceX; i += EinheitsValue)
    {

        float skalar = i / distanceX; //x / 1 full
        FVector2D newPos = FVector2DFourAnchorBezier(p0, p1, p2, p3, skalar);
        output.push_back(newPos);
    }
}





FVector2D bezierCurve::FVector2DFourAnchorBezier(
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
    return abbcbccd;
}




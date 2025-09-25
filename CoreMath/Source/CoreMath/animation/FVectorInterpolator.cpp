#include "FVectorInterpolator.h"



FVector FVectorInterpolator::interpolate(float DeltaTime){
    if(reached){
        return target;
    }
    deltaTime += DeltaTime;
    float skalarCurrent = skalar();
    UpdateReachedFlag(skalarCurrent);
    if(reached){
        return target;
    }
    
    FVector gx; //= interpolation(from, target, skalarCurrent);
    if(useHermiteSplineFlag){
        gx = HermiteInterpolate(skalarCurrent); //new debug test
    }else{
        gx = interpolation(from, target, skalarCurrent);
    }

    //--- Not in use ---
    //wenn die richtungs vektoren anti paralell zu einander liegen
    //kann ich prüfen ob mein punkt passiert wurde
    
    //FVector connect = target - from; // AB = B - A  
    //FVector dirToB = target - interpolated;
    //float dotProduct = FVector::DotProduct(dirToB.GetSafeNormal(), connect.GetSafeNormal());

    return gx;
}


float FVectorInterpolator::Distance(FVector &a, FVector &b){
    return FVector::Dist(a, b);
}

FVector FVectorInterpolator::HermiteInterpolate(float skalar){
    //define tangents for start and end velocity
    FVector tangentA(10, 0, 0); //0,0,100 makes funny yeet up
    FVector tangentB = (target - from).GetSafeNormal() * 10.0f;

    return HermiteInterpolate(from, target, tangentA, tangentB, skalar);
}

FVector FVectorInterpolator::HermiteInterpolate(
    FVector &p0, //p0 line start
    FVector &p1, //p1 line end
    FVector &m0, //tangente at p0
    FVector &m1, //tangente at p1
    float t
){
    /**
     *  f(t) = at^3 + bt^2 + ct + d
     * 
     *  hermite spline known formula
     * 	h_0(t) = 2t^3 - 3t^2 + 1
     *  h_1(t) = -2t^3 + 3t^2
	 *  h_2(t) = t^3 - 2t^2 + t
     *  h_3(t) = t^3 - t^2
     * 
     *  hermite(t) = h0 * p0 + h1 * p1 + h2 * tangent1 + h3 * tangent3
     * 
     * 
     */


    float t2 = t * t;
    float t3 = t2 * t;

    float h0 = 2 * t3 - 3 * t2 + 1; //f(0) = 1 --> p0
    float h1 = -2 * t3 + 3 * t2;    //f(1) = -2 * 1 + 3 * 1 = 1 --> p1

    float h2 = t3 - 2 * t2 + t;
    float h3 = t3 - t2;

    return (p0 * h0) + (p1 * h1) + (m0 * h2) + (m1 * h3);
}








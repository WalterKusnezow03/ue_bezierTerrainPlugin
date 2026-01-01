#include "QuadIntersectFrame.h"


FQuadIntersectFrame::FQuadIntersectFrame(){

}

FQuadIntersectFrame::~FQuadIntersectFrame(){

}

FQuadIntersectFrame::FQuadIntersectFrame(const FQuadIntersectFrame &other){
    if(this != &other){
        *this = other;
    }
}
FQuadIntersectFrame &FQuadIntersectFrame::operator=(const FQuadIntersectFrame &other){
    if(this != &other){
        plane = other.plane;
        rotationInverse = other.rotationInverse;

        v0Projected = other.v0Projected;
        v1Projected = other.v1Projected;
        v2Projected = other.v2Projected;
        v3Projected = other.v3Projected;

        v0Normal = other.v0Normal;
        v1Normal = other.v1Normal;
        v2Normal = other.v2Normal;
        v3Normal = other.v3Normal;
    }
    return *this;
}

void FQuadIntersectFrame::Setup(
    const FVector &v0,
    const FVector &v1,
    const FVector &v2,
    const FVector &v3
){
    FTriangleIntersectFrame::Setup(v0, v1, v2); //include normal calculation

    v3Projected = rotationInverse * v3;
    v3Projected.Z = 0.0f;
    CalculateNormals();
}

void FQuadIntersectFrame::CalculateNormals(){
    v0Normal = CalculateNormal(v0Projected, v1Projected);
    v1Normal = CalculateNormal(v1Projected, v2Projected);
    v2Normal = CalculateNormal(v2Projected, v3Projected);
    v3Normal = CalculateNormal(v3Projected, v0Projected);
}







bool FQuadIntersectFrame::DoesIntersect(
    const FVector &start,
    const FVector &dir,
    FVector &outIntersectionPoint
){
    /*
    FMath::RayPlaneIntersection
    static TVector<T> RayPlaneIntersection(
        const TVector<T>& RayOrigin,
        const TVector<T>& RayDirection,
        const TPlane<T>& Plane
    );
    */
    FVector Intersection = FMath::RayPlaneIntersection(start, dir, plane);
    if(Intersection.ContainsNaN()){
        return false;
    }

    FVector IntersectionProjected = rotationInverse * Intersection;
    DebugHelper::logMessage(
        FString::Printf(
            TEXT("FTriangleIntersectFrame (Quad) intersection original (%s) projected (%s)"),
            *Intersection.ToString(),
            *IntersectionProjected.ToString()
        )
    );
    if(IntersectionProjected.ContainsNaN()){
        return false;
    }


    IntersectionProjected.Z = 0.0f; //only interested in projected 2D pure.

    //check right left test triangle
    FVector relativeToV0 = IntersectionProjected - v0Projected; // AB = B - A
    FVector relativeToV1 = IntersectionProjected - v1Projected;
    FVector relativeToV2 = IntersectionProjected - v2Projected;
    FVector relativeToV3 = IntersectionProjected - v3Projected;

    bool signv0 = FVector::DotProduct(v0Normal, relativeToV0) >= 0.0f;
    bool signv1 = FVector::DotProduct(v1Normal, relativeToV1) >= 0.0f;
    bool signv2 = FVector::DotProduct(v2Normal, relativeToV2) >= 0.0f;
    bool signv3 = FVector::DotProduct(v3Normal, relativeToV3) >= 0.0f;
    //inside triangle by right left test (should be correct.)
    bool result = signv0 == signv1 && signv1 == signv2 && signv2 == signv3;
    if(result){
        outIntersectionPoint = Intersection;
    }

    return result;

}
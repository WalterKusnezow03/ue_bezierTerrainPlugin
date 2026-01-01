#include "TriangleIntersectFrame.h"
#include "DebugPlugin/DebugHelper.h"

FTriangleIntersectFrame::FTriangleIntersectFrame(){

}

FTriangleIntersectFrame::~FTriangleIntersectFrame(){

}

FTriangleIntersectFrame::FTriangleIntersectFrame(
    const FTriangleIntersectFrame &frame
){
    if(this != &frame){
        *this = frame;
    }
}

FTriangleIntersectFrame &FTriangleIntersectFrame::operator=(
    const FTriangleIntersectFrame &frame
){
    if(this != &frame){
        plane = frame.plane;
        rotationInverse = frame.rotationInverse;

        v0Projected = frame.v0Projected;
        v1Projected = frame.v1Projected;
        v2Projected = frame.v2Projected;

        v0Normal = frame.v0Normal;
        v1Normal = frame.v1Normal;
        v2Normal = frame.v2Normal;

        v0Index = frame.v0Index;
        v1Index = frame.v1Index;
        v2Index = frame.v2Index;
    }
    return *this;
}

void FTriangleIntersectFrame::Setup(
    const FVector &v0,
    const FVector &v1,
    const FVector &v2
){
    FVector normal = FVector::CrossProduct((v1 - v0), (v2 - v0));
    normal = normal.GetSafeNormal();

    //default:
    FVector2D XAxis(1,0); //forward
    FVector2D ZAxis(0,1); //Up

    //faster immidiate rotation
    //+pitch pi/2
    //FVector2D XAxis(0,1); //forward * r(90)
    //FVector2D ZAxis(-1,0); //Up * r(90)

    rotationInverse = MMatrix::createRotatorFrom(normal, XAxis, ZAxis);
    rotationInverse = rotationInverse.transposedRotation();//R^T = R^-1
    //rotationInverse.pitchRadAdd(MMatrix::degToRadian(90)); // pitch up look up from x(1,0,0) to (0,0,1)

    DebugHelper::logMessage(TEXT("FTriangleIntersectFrame --- log start ---"));
    FVector normalDebug = rotationInverse * normal;
    //inverse ok.
    DebugHelper::logMessage(
        FString::Printf(
            TEXT("FTriangleIntersectFrame check rotator (%s) normal Deprojected (%s)"),
            *rotationInverse.extractRotator().ToString(),
            *normalDebug.ToString()
        )
    );



    



    v0Projected = rotationInverse * v0;
    v1Projected = rotationInverse * v1;
    v2Projected = rotationInverse * v2;

    //log
    DebugHelper::logMessage(
        FString::Printf(
            TEXT("FTriangleIntersectFrame original (%s) projected (%s)"),
            *v0.ToString(),
            *v0Projected.ToString()
        )
    );
    DebugHelper::logMessage(
        FString::Printf(
            TEXT("FTriangleIntersectFrame original (%s) projected (%s)"),
            *v1.ToString(),
            *v1Projected.ToString()
        )
    );
    DebugHelper::logMessage(
        FString::Printf(
            TEXT("FTriangleIntersectFrame original (%s) projected (%s)"),
            *v2.ToString(),
            *v2Projected.ToString()
        )
    );


    v0Projected.Z = 0.0f;
    v1Projected.Z = 0.0f;
    v2Projected.Z = 0.0f;



    CalculateNormals();
    plane = FPlane(v0, normal);

}

void FTriangleIntersectFrame::CalculateNormals(){
    v0Normal = CalculateNormal(v0Projected, v1Projected);
    v1Normal = CalculateNormal(v1Projected, v2Projected);
    v2Normal = CalculateNormal(v2Projected, v0Projected);
}

FVector FTriangleIntersectFrame::CalculateNormal(FVector &v0, FVector &v1){
    FVector connect = v1 - v0;
    FVector normal2D(connect.Y * -1.0f, connect.X, 0.0f);
    return normal2D;
}

bool FTriangleIntersectFrame::DoesIntersect(
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
    FVector IntersectionProjected = rotationInverse * Intersection;
    //IntersectionProjected.Z = 0.0f; //only interested in projected 2D pure.

    DebugHelper::logMessage(
        FString::Printf(
            TEXT("FTriangleIntersectFrame intersection original (%s) projected (%s)"),
            *Intersection.ToString(),
            *IntersectionProjected.ToString()
        )
    );

    //check right left test triangle
    FVector relativeToV0 = IntersectionProjected - v0Projected; // AB = B - A
    FVector relativeToV1 = IntersectionProjected - v1Projected;
    FVector relativeToV2 = IntersectionProjected - v2Projected;

    bool signv0 = FVector::DotProduct(v0Normal, relativeToV0) >= 0.0f;
    bool signv1 = FVector::DotProduct(v1Normal, relativeToV1) >= 0.0f;
    bool signv2 = FVector::DotProduct(v2Normal, relativeToV2) >= 0.0f;
    bool result = signv0 == signv1 && signv1 == signv2; //inside triangle by right left test (should be correct.)
    if(result){
        outIntersectionPoint = Intersection;
    }

    return result;
}





/// identity check
void FTriangleIntersectFrame::SetIdentifier(int32 v0, int32 v1, int32 v2){
    v0Index = v0;
    v1Index = v1;
    v2Index = v2;
}
bool FTriangleIntersectFrame::IsSameIdentifier(int32 v0, int32 v1, int32 v2){
    return v0 == v0Index && v1 == v1Index && v2 == v2Index;
}

bool FTriangleIntersectFrame::HasIdentifier(int32 v0){
    return v0 == v0Index || v0 == v1Index || v0 == v2Index;
}

void FTriangleIntersectFrame::CopyIdentifier(int32 &v0, int32 &v1, int32 &v2){
    v0 = v0Index;
    v1 = v1Index;
    v2 = v2Index;
}

void FTriangleIntersectFrame::IncreaseIdentifierBy(int32 offset){
    v0Index += offset;
    v1Index += offset;
    v2Index += offset;
}
#include "MPolygon.h"
#include "Math/UnrealMathUtility.h"
#include "CoreMath/algorithm/GrahamScan/GrahamScan2D.h"
#include "CoreMath/Debug/CoreMathDebugHelper.h"


MPolygon::MPolygon(){

}

MPolygon::~MPolygon(){

}
MPolygon::MPolygon(const MPolygon &other){
    if(this != &other){
        *this = other;
    }
}
MPolygon &MPolygon::operator=(const MPolygon &other){
    if(this != &other){
        myTransform = other.myTransform;

        shapeTransformed = other.shapeTransformed;
        shapeOriginal = other.shapeOriginal;

        UpdateBounds();

        //might not be needed.
        moveAwayFromIntersection = other.moveAwayFromIntersection;
    }
    return *this;
}

void MPolygon::SetShape(const TArray<FVector2D> &shapeIn){
    shapeOriginal = shapeIn;
    UpdateTransformedShape();
    UpdateBounds();
    CalculateArea();
}

void MPolygon::SetShape(const TArray<FVector2D> &shapeIn, int minDistanceBetweenPoints){
    shapeOriginal = IncreaseDetail(shapeIn, minDistanceBetweenPoints);

    UpdateTransformedShape();
    UpdateBounds();
    CalculateArea();
}


TArray<FVector2D> MPolygon::IncreaseDetail(const TArray<FVector2D> &shapeIn, int minDistanceBetweenPoints){

    TArray<FVector2D> newShape;
    minDistanceBetweenPoints = minDistanceBetweenPoints * minDistanceBetweenPoints;

    for (int i = 1; i < shapeIn.Num(); i++)
    {
        const FVector2D &prev = shapeIn[i - 1];
        const FVector2D &current = shapeIn[i];
        newShape.Add(prev);
        float gap = FVector2D::DistSquared(prev, current);
        if(gap > minDistanceBetweenPoints){
            FVector2D dir = current - prev; //AB = B - A
            float tStep = minDistanceBetweenPoints / gap;
            for (float t = 0.0f; t < 1.0f; t += tStep){
                FVector2D gt = prev + t * dir;
                newShape.Add(gt);
            }
            //CoreMathDebugHelper::logMessage("SetShape added detail");
        }
        newShape.Add(current);
    }
    return newShape;
}

void MPolygon::SetTransform(const MMatrix2D &transform){
    if(shapeOriginal.Num() <= 0){
        return;
    }
    locationCopy = transform.getTranslation();
    myTransform = transform;

    shapeTransformed.SetNum(shapeOriginal.Num());
    for (int i = 0; i < shapeOriginal.Num(); i++){
        FVector2D copy = shapeOriginal[i];
        shapeTransformed[i] = transform * copy;
    }
    UpdateBounds();
}


void MPolygon::UpdateTransformedShape(){
    if(shapeOriginal.Num() <= 0){
        return;
    }
    SetTransform(myTransform);
}

void MPolygon::CalculateArea(){

    areaFound = localBounds.sizeX() * localBounds.sizeY();
    return;

    /*
    // deprecated, can be found from bounds
    if(shapeOriginal.Num() <= 1){
        areaFound = 0.0f;
        return;
    }
    if(shapeOriginal.Num() > 0){
        FVector2D min = shapeOriginal[0];
        FVector2D max = min;
        for (int i = 1; i < shapeOriginal.Num(); i++){
            FVector2D &current = shapeOriginal[i];
            min.X = std::min(min.X, current.X);
            min.Y = std::min(min.Y, current.Y);

            max.X = std::max(max.X, current.X);
            max.Y = std::max(max.Y, current.Y);
        }

        areaFound = (max.X - min.X) * (max.Y - min.Y);
    }*/
}



bool MPolygon::DoesIntersectIncludingBoundsCheck(const MPolygon &shapeIn){
    if(IsInBound(shapeIn)){
        //CoreMathDebugHelper::logMessage("MPolygon Child Bound hit");
        return true;
    }
    return DoesIntersect(shapeIn);
}

bool MPolygon::DoesIntersect(
    const MPolygon &other
){
    return DoesIntersectClockwiseShape(other.shapeTransformed);
}

bool MPolygon::DoesIntersectClockwiseShape(const TArray<FVector2D> &shapeToFit){
    if(shapeToFit.Num() <= 0 || shapeTransformed.Num() <= 0){
        return false;
    }

    for (int i = 0; i < shapeToFit.Num(); i++){
        int j = (i + 1) % shapeToFit.Num();
        if(DoesIntersect(shapeToFit[i], shapeToFit[j])){
            return true;
        }
    }
    return false;
}




bool MPolygon::DoesIntersect(
    const FVector2D &aWorld, 
    const FVector2D &bWorld
){
    if(shapeTransformed.Num() <= 0){
        return false;
    }

    for (int i = 0; i < shapeTransformed.Num(); i++){
        int j = (i + 1) % shapeTransformed.Num();
        if(DoesIntersect(
            aWorld, bWorld,
            shapeTransformed[i], shapeTransformed[j]
        )){
            return true;
        }
    }
    return false;
}

bool MPolygon::DoesIntersect(
    const FVector2D &aWorld, 
    const FVector2D &bWorld,
    TArray<FVector2D> &intersectionsOut
){
    if (shapeTransformed.Num() <= 0)
    {
        return false;
    }

    intersectionsOut.Empty();
    for (int i = 0; i < shapeTransformed.Num(); i++){
        int j = (i + 1) % shapeTransformed.Num();
        FVector2D hit;
        if(DoesIntersect(
            aWorld, bWorld,
            shapeTransformed[i], shapeTransformed[j],
            hit
        )){
            if(!intersectionsOut.Contains(hit)){
                intersectionsOut.Add(hit);
            }
            
        }
    }
    return intersectionsOut.Num() > 0;
}



FVector2D MPolygon::EdgeNormal(const FVector2D &v0, const FVector2D &v1){
    //is a clock wise rotation, (from left to right)
    //means the point with dot product < 0, is outside the polygon if shape is clock wise sorted
    FVector2D connect = v1 - v0;
    return EdgeNormal(connect);
}

FVector2D MPolygon::EdgeNormal(const FVector2D &connect){
    FVector2D edgeNormal(connect.Y, -connect.X);
    return edgeNormal.GetSafeNormal();
}




bool MPolygon::DoesIntersect(
    const FVector2D &aWorld, //edge 1
    const FVector2D &bWorld,
    const FVector2D &e1,     //edge 2
    const FVector2D &e2
){

    FVector2D ignored;
    return DoesIntersect(
        aWorld,
        bWorld,
        e1,
        e2,
        ignored
    );
}

bool MPolygon::DoesIntersect(
    const FVector2D &aWorld, //edge 1
    const FVector2D &bWorld,
    const FVector2D &e1,     //edge 2
    const FVector2D &e2,
    FVector2D &outIntersect
){

    FVector2D edgeNormal = EdgeNormal(aWorld, bWorld); 

    FVector2D e1Local = e1 - aWorld; //kante ins relative system bringen.
    FVector2D e2Local = e2 - aWorld;

    // ---  ---
    FVector2D IntersectionPoint;
    if (SegmentIntersection2D(aWorld, bWorld, e1, e2, IntersectionPoint))
    {
        //since an inetrsection is detected, one point must be on the negative side of the normal.
        bool e1Outside = FVector2D::DotProduct(edgeNormal, e1Local) < 0.0; //assuming e1 is outside of clockwise ordered shape
        const FVector2D &outsidePoint = e1Outside ? e1 : e2;

        outIntersect = IntersectionPoint;

        // AB = B - A, transform needed inwards to not intersect
        //(might not be needed for the algorythm)
        moveAwayFromIntersection = IntersectionPoint - outsidePoint;

        return true;
    }

    return false;
}






bool MPolygon::SegmentIntersection2D(
    const FVector2D &a0_2D,
    const FVector2D &a1_2D,
    const FVector2D &b0_2D,
    const FVector2D &b1_2D,
    FVector2D &outIntersection_2D
){
    if(IntersectsPoint(a0_2D, a1_2D, b0_2D)){
        outIntersection_2D = b0_2D;
        return true;
    }

    if(IntersectsPoint(a0_2D, a1_2D, b1_2D)){
        outIntersection_2D = b1_2D;
        return true;
    }

    FVector a0(a0_2D.X, a0_2D.Y, 0.0);
    FVector a1(a1_2D.X, a1_2D.Y, 0.0);
    FVector b0(b0_2D.X, b0_2D.Y, 0.0);
    FVector b1(b1_2D.X, b1_2D.Y, 0.0);


    //make line extended because hit thru point is not 
    //counted a intersction in FMath!
    //ExtendLineInBothDirections(a0, a1, sizeExtensionCm);
    //ExtendLineInBothDirections(b0, b1, sizeExtensionCm);

    //bool SegmentIntersection2D(
    //  const FVector& SegmentStartA, 
    //  const FVector& SegmentEndA, 
    //  const FVector& SegmentStartB, 
    //  const FVector& SegmentEndB, 
    //  FVector& out_IntersectionPoint
    //);
    FVector IntersectionPoint;
    if(FMath::SegmentIntersection2D(a0, a1, b0, b1, IntersectionPoint)){
        outIntersection_2D.X = IntersectionPoint.X;
        outIntersection_2D.Y = IntersectionPoint.Y;
        return true;
    }


    return false;
}




bool MPolygon::IntersectsPoint(
    const FVector2D &a0,
    const FVector2D &a1,
    const FVector2D &b
){
    FVector2D dir = a1 - a0;
    FVector2D dirComapre = b - a0;

    if(FVector2D::DotProduct(dir.GetSafeNormal(), dirComapre.GetSafeNormal()) <= 0.99999f){
        return false;
    }

    //-- works if paralell test is sucess! --
    //gx = a + t(b-a), t in 0, 1 for a to b range

    //a0 + t * (a1-a0) = b || solve for t
    //t * (a1-a0) = b - a0 ||^2
    //t^2 * (a1-a0)^2 = (b - a0)^2
    //t^2 = (b - a0)^2 / (a1-a0)^2
    //t1,2 = sqrt((b - a0)^2 / (a1-a0)^2)

    
    FVector2D ab = b - a0;
    float ab2 = ab.X * ab.X + ab.Y * ab.Y;

    FVector2D a0a1 = a1 - a0;
    float a0a12 = a0a1.X * a0a1.X + a0a1.Y * a0a1.Y;

    float innerSqrt = ab2 / a0a12;
    float sqrt = std::sqrt(innerSqrt);

    float t1 = sqrt;
    float t2 = -1.0f * sqrt;
    if((t1 >= 0.0f && t1 <= 1.0f) || (t2 >= 0.0f && t2 <= 1.0f)){
        /*CoreMathDebugHelper::logMessage(
            FString::Printf(
                TEXT("MPolygon was hitting line!%.2f, a0:%s a1:%s b:%s"),
                std::max(t1,t2), *a0.ToString(), *a1.ToString(), *b.ToString()
            )
        );*/
        return true;
    }
    return false;
}



//experimental
TArray<FVector2D> MPolygon::facingParentCenter(FVector2D &centerParent){
    FVector2D center = centerOfTransformedShape();
    FVector2D edge = centerParent - center; //AB = B - A

    TArray<FVector2D> rightOff;

    bool distanceBased = true;
    if(distanceBased){
        float distCenter = FVector2D::DistSquared(center, centerParent);
        for (int i = 0; i < shapeTransformed.Num(); i++){
            const FVector2D& current = shapeTransformed[i];
            float distVertex = FVector2D::DistSquared(current, centerParent);
            
            if (distVertex < distCenter){
                rightOff.Add(current);
            }
        }
        return rightOff;
    }

    for (int i = shapeTransformed.Num() - 1; i >= 0; i--){
        FVector2D &current = shapeTransformed[i];
        FVector2D relativeToCenter = current - center; // AB = B - A
        if(FVector2D::DotProduct(edge, relativeToCenter) >= 0.0f){ //>=0
            rightOff.Add(current);
        }
    }
    return rightOff;
}







FVector2D MPolygon::centerOfTransformedShape(){
    FVector2D result;
    for (int i = 0; i < shapeTransformed.Num(); i++){
        result += shapeTransformed[i];
    }
    if(shapeTransformed.Num() > 0){
        result /= shapeTransformed.Num();
    }
    return result;
}



// --- bounds ---
bool MPolygon::IsInBound(const MPolygon &polygon){

    //show bounds here.
    //make bounds test in test algorythm!

    if(myBounds.OtherIsInside(polygon.myBounds)){
        //CoreMathDebugHelper::logMessage(TEXT("--MPolygon Bounds test--"));
        //CoreMathDebugHelper::logMessage(TEXT("MPolygon bound outer"), myBounds.ToString());
        //CoreMathDebugHelper::logMessage(TEXT("MPolygon bound inner"), polygon.myBounds.ToString());
        return true;
    }
    return false;

    //return myBounds.OtherIsInside(polygon.myBounds);
}

void MPolygon::UpdateBounds(){
    myBounds.Reset();
    //Update(const TArray<FVector2D> &buffer);
    myBounds.Update(internalTransformed());

    localBounds.Reset();
    localBounds.Update(shapeOriginal);
}

/// ---- find closest vertecies to center coordinate, add them ----

void MPolygon::AppendClosestTo(FVector2D &center, TArray<FVector2D> &updatedShape){
    //es müssen die jenigen vertecies gefunden werden
    //die am nächsten am zentrum liegen

    //am einfachsten ist es, das zentrum dieses polygons zu finden
    //es orthogonal zur normale zu spalten, und jene vertecies als valide anzunehmen
    //und diese im urhzeiger sinn zu sortieren

    FVector2D ownCenter = centerOfTransformedShape();
    FVector2D toCenter = center - ownCenter;

    //um 90 grad neigen und alle
    TArray<FVector2D> rightOffUnsorted; //unknown order
    for (int i = 0 ; i < shapeTransformed.Num(); i++){
        
        FVector2D &current = shapeTransformed[i];
        FVector2D relativeToCenter = current - ownCenter; // AB = B - A
        if(FVector2D::DotProduct(relativeToCenter, toCenter) >= 0.0f){ //>=0
            rightOffUnsorted.Add(current);
        }
    }

    GrahamScan2D scanner;
    scanner.ComputeConvexHull(rightOffUnsorted);
    updatedShape.Append(rightOffUnsorted);
}




// --- new: Append right side bounds to array for new inner shape to fill ---
void MPolygon::AppendRightSideBounds(TArray<FVector2D> &appendTo){
    TArray<FVector2D> bound = myBounds.asVertecies();
    /*
    FBoundingBox2D::asVertecies(){
    /// @brief min coordinate on x and y
    /// FVector2D topLeft;
    /// @brief max coordiate on x and y
    /// FVector2D bottomRight;
    TArray<FVector2D> output = {
        topLeft,
        FVector2D(topLeft.X, bottomRight.Y),
        bottomRight,
        FVector2D(bottomRight.X, topLeft.Y),
    };
    return output;
    }*/
    appendTo.Add(bound[2]);
    appendTo.Add(bound[3]);
}

const FBoundingBox2D &MPolygon::boundingBox() const {
    return myBounds;
}

const FBoundingBox2D &MPolygon::LocalBoundingBox() const {
    return localBounds;
}


FBoundingBox2D MPolygon::localBoundsTransformed(){
    FBoundingBox2D copy = localBounds;

    //transform vertecies but keep their logic of top left, top right etc
    copy.topLeft = myTransform * copy.topLeft;
    copy.bottomRight = myTransform * copy.bottomRight;
    return copy;
}

FString MPolygon::ToString(){
    FString msg = TEXT("MPolygon tostring original(");
    for(FVector2D &current : shapeOriginal){
        msg += current.ToString();
        msg += TEXT(", ");
    }
    msg += TEXT(") transformed (");
    for(FVector2D &current : shapeTransformed){
        msg += current.ToString();
        msg += TEXT(", ");
    }
    msg += TEXT(")");
    return msg;
}
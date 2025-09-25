#include "GreedyFilledPolygon.h"
#include "CoreMath/Debug/CoreMathDebugHelper.h"
#include <map>

GreedyFilledPolygon::GreedyFilledPolygon(){

}

GreedyFilledPolygon::~GreedyFilledPolygon(){

}

int GreedyFilledPolygon::PolygonCount(){
    return internalPolygonsAdded.Num();
}

/// expects polygon to have its pivot at 0,0,0
void GreedyFilledPolygon::Add(TArray<MPolygon> &array){
    //TODO: Es könnten alle polygone längs rotiert werden

    //shape must be appended by detail!!! DETAIL INCREASE!

    //ich möchte hier an der kante entlang wandern, und versuchen polygone,
    //anhand der kante die in eine richtung zeigt, 
    //und der y höhe des bounds des polygons enspricht, es versuchen zu fitten

    int maxTries = 3 * array.Num();
    int tryCount = 0;

    int indexInShape = 0; //running over outline, try to add polygons
    int prevIndex = 0;

    int sameIndexCount = 0;
    int maxSameIndex = 3;
    std::map<int, bool> map;

    // polygone so lange versuchen einzufügen bis alle weg sind

    ///count polygons before closing loop
    int addedCount = 0;
    for (int i = 0; i < array.Num(); i++)
    {
        if(shapeTransformed.Num() <= 0){
            return;
        }

        if(map.find(i) == map.end()){
            map[i] = false;
        }

        bool added = map[i];
        if(!added){
            MPolygon &p = array[i];

            prevIndex = indexInShape;
            int resultIndex = AddAndReturnNextIndex(p, indexInShape);
            if(resultIndex > indexInShape){
                //sucessfully added
                map[i] = true;
                addedCount++;
            }


            CoreMathDebugHelper::logMessage(
                FString::Printf(TEXT("GreedyFilledPolygon: result Index %d of %d"), 
                resultIndex,
                VertexCount()
            ));

            if(false && SkipNeeded(prevIndex, resultIndex, sameIndexCount, maxSameIndex)){
                resultIndex++;
            }

            //if (resultIndex >= VertexCount() || prevIndex > resultIndex)
            if(ShrinkNeeded(prevIndex, resultIndex))
            {
                ShrinkShapeFromRightOffVertecies(addedCount, resultIndex);
                resultIndex = 0;
                addedCount = 0;

                //debug:
                if(true){
                    return;
                }

                //TODO!

                // shrink shape would be needed here.
                CoreMathDebugHelper::logMessage("GreedyFilledPolygon: Shrink Would be needed!");
            }
            indexInShape = resultIndex;
        }
    }
}

bool GreedyFilledPolygon::SkipNeeded(
    int prevIndex,
    int resultIndex,
    int &sameIndexCount,
    int maxSameIndex
){
    if (resultIndex == prevIndex)
    {
        sameIndexCount++;
        if(sameIndexCount >= maxSameIndex){
            sameIndexCount = 0;
            return true;
        }
    }
    return false;
}

bool GreedyFilledPolygon::ShrinkNeeded(
    int prevIndex,
    int resultIndex
){
    if(resultIndex >= VertexCount() || prevIndex > resultIndex){
        return true;
    }
    if(resultIndex >= VertexCount() * 0.6f){
        return true;
    }
    return false;
}

///tries to fit next polygon at all cost.
///next index for next polygon is returned
/// expects polygon to have its pivot at 0,0,0
int GreedyFilledPolygon::AddAndReturnNextIndex(MPolygon &p, int i){
    if(shapeTransformed.Num() <= 0){
        return i;
    }

    //from index i, the next vertex is searched to fit the height of the polygon to add.
    int next = i;

    const FBoundingBox2D &ref = p.boundingBox();
    float height = ref.sizeY();
    float heightSquared = height * height;

    FVector2D &startingPivot = shapeTransformed[ValidateIndex(i)];

    //find next point fitting height, or use scalar.
    for (int j = i + 1; j < VertexCount(); j++){
        FVector2D &current = shapeTransformed[j];
        if(FVector2D::DistSquared(startingPivot, current) >= heightSquared){
            //CoreMathDebugHelper::logMessage("GreedyFilledPolygon: Try Fit Y ok");


            MMatrix2D tr = MakeTransform(startingPivot, current);
            p.SetTransform(tr);

            int maxAdjustments = 100;
            float width = ref.sizeX();
            float step = width / maxAdjustments;

            for (int a = 0; a < maxAdjustments; a++){
                MMatrix2D m;
                if (DoesIntersect(p))
                {   
                    //CoreMathDebugHelper::logMessage("GreedyFilledPolygon: Fit failed");

                    //M = T * R * Tlocal <-- lese richtung --
                    float scalar = step * a;
                    MMatrix2D tLocal;
                    tLocal.SetTranslation(scalar * FVector2D(1.0f, 1.0f));
                    /*CoreMathDebugHelper::logMessage(
                        "GreedyFilledPolygon: tlocal: ", 
                        tLocal.asString()
                    );*/

                    m = tr * tLocal;
                    p.SetTransform(m);

                    /*CoreMathDebugHelper::logMessage(
                        "GreedyFilledPolygon: M: ", 
                        tLocal.asString()
                    );*/
                }
                else
                {
                    /*CoreMathDebugHelper::logMessage(
                        "GreedyFilledPolygon: Fit sucess, location: ", 
                        p.locationCopy.ToString()
                    );*/
                    internalPolygonsAdded.Add(p);

                    next = j + 1; //update to start at reached vertex, or:
                    //next = std::max(FindClosestIndexToTopLeft(p), next);

                    return next;
                }
            }

        }
    }
    return next; //same index returned if adding failed
}


MMatrix2D GreedyFilledPolygon::MakeTransform(FVector2D &startingPivot, FVector2D &current){

    //fit polygon, translation might be needed
    FVector2D dirAlongY = current - startingPivot;

    //CoreMathDebugHelper::logMessage("GreedyFilledPolygon: Dir Y:", dirAlongY.ToString());
    
    MMatrix2D R;
    FVector2D yAxis(0,1);
    R.SetRotation(yAxis, dirAlongY);
    /*CoreMathDebugHelper::logMessage(
        "GreedyFilledPolygon: rotationmatrix: ", 
        R.asString()
    );*/

    MMatrix2D T;
    T.SetTranslation(startingPivot);
    /*CoreMathDebugHelper::logMessage(
        "GreedyFilledPolygon: translation: ",
        T.asString());*/

    MMatrix2D tr = T * R;
    return tr;

    /*CoreMathDebugHelper::logMessage(
        "GreedyFilledPolygon: T * R: ", 
        tr.asString()
    );*/
}

int GreedyFilledPolygon::FindClosestIndexToTopLeft(MPolygon &addedPolygon){
    FBoundingBox2D bounds = addedPolygon.localBoundsTransformed(); //in transformed space
    
    //looking for top left corner - local bounds transformed.
    FVector2D topLeft = bounds.minXmaxY();
    int index = FindClosestIndexTo(topLeft);

    if(index >= 0 && index < shapeTransformed.Num()){
        FVector2D &found = shapeTransformed[index];
        FString message = FString::Printf(
            TEXT("GreedyFilledPolygon Closest Vertex: %s to %s"),
            *found.ToString(),
            *topLeft.ToString()
        );
        //CoreMathDebugHelper::logMessage(message);
    }
    return index;
}

int GreedyFilledPolygon::FindClosestIndexTo(const FVector2D &target){
    if(shapeTransformed.Num() > 0){
        float dist = FVector2D::DistSquared(target, shapeTransformed[0]);
        int iFound = 0;
        for (int i = 1; i < shapeTransformed.Num(); i++)
        {   
            float newDist = FVector2D::DistSquared(target, shapeTransformed[i]);
            if(newDist < dist){
                iFound = i;
                dist = newDist;
            }
        }
        return iFound;
    }
    return -1;
}





bool GreedyFilledPolygon::IndexIsValid(int i, TArray<MPolygon> &polygons){
    return i >= 0 && i < polygons.Num();
}

int GreedyFilledPolygon::ValidateIndex(int index){
    if(shapeTransformed.Num() <= 0){
        return 0;
    }
    index %= shapeTransformed.Num();
    if(index < 0){
        index += shapeTransformed.Num();
    }
    return index;
}


void GreedyFilledPolygon::PrintOutline(){
    FString result = TEXT("GreedyFilledPolygon vectordata(");
    for (int i = 0; i < shapeTransformed.Num(); i++){
        result += shapeTransformed[i].ToString();
        if(i < shapeTransformed.Num() - 1){
            result += TEXT(" , ");
        }
    }
    result += TEXT(")");
    CoreMathDebugHelper::logMessage(result);
}


void GreedyFilledPolygon::AppendAllPolygonsIncludingSelfIntoArray(TArray<MPolygon> &outArray){
    outArray.Add(*this); //will introduce object slice which is expected.
    outArray.Append(internalPolygonsAdded);
}




// -- intersect override: with self and aded shapes.
bool GreedyFilledPolygon::DoesIntersect(const MPolygon &shapeIn){

    //if vertecies are modified, check like this, otherwise just bounds because the transform vertecies
    //are still modifed to get anchor points! - hack.
    if(MPolygon::DoesIntersect(shapeIn)){
        //CoreMathDebugHelper::logMessage("GreedyFilledPolygon intersect reason: own hit ");
        return true;
    }
    if(!IsInBound(shapeIn)){
        //CoreMathDebugHelper::logMessage("GreedyFilledPolygon intersect reason: bound hit ");
        return true;
    }
    for (int i = 0; i < internalPolygonsAdded.Num(); i++){
        MPolygon &current = internalPolygonsAdded[i];
        if(current.DoesIntersectIncludingBoundsCheck(shapeIn)){
            //CoreMathDebugHelper::logMessage("GreedyFilledPolygon intersect reason: inetrnal child hit ");
            return true;
        }
    }
    return false;
}






void GreedyFilledPolygon::ShrinkShapeFromRightOffVertecies(int addedCount, int reachedUntilIndex){
    if(addedCount <= 0){
        return;
    }

    //PrintOutline();
    
    //copy vertecies facing to inwards
    FVector2D center = centerOfTransformedShape();

    TArray<FVector2D> innerHullFromBounds;
    int offset = internalPolygonsAdded.Num() - std::abs(addedCount);
    for (int i = offset; i < internalPolygonsAdded.Num(); i++){
        MPolygon &current = internalPolygonsAdded[i];
        innerHullFromBounds.Append(current.facingParentCenter(center));
    }

    //copy vertecies which were not touched during running one round around polygon
    int difference = shapeTransformed.Num() - reachedUntilIndex;
    offset = innerHullFromBounds.Num();
    innerHullFromBounds.SetNumUninitialized(offset + difference);
    for (int i = 0; i < difference; i++){
        innerHullFromBounds[offset + i] = shapeTransformed[reachedUntilIndex + i];
    }

    GrahamScan2D scan;
    scan.ComputeConvexHull(innerHullFromBounds);
    SetShapeTransformed(innerHullFromBounds, 10.0f);
    //PrintOutline();
    
    

}

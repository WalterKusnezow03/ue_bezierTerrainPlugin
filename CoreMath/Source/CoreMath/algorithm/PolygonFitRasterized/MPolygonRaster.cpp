#include "MPolygonRaster.h"
#include "CoreMath/Debug/CoreMathDebugHelper.h"
#include "CoreMath/algorithm/PolygonFitRasterized/FitCache/MPolygonRasterFitCache.h"


MPolygonRaster::MPolygonRaster(){
    SetMinSizePolygon(2, 2); //some value to be set
}

MPolygonRaster::~MPolygonRaster(){

}

MPolygonRaster::MPolygonRaster(const MPolygonRaster &other){
    if(this != &other){
        *this = other;
    }
}

MPolygonRaster &MPolygonRaster::operator=(const MPolygonRaster &other){
    if(this != &other){
        shape = other.shape;
        freePositionsCache = other.freePositionsCache;
        polygonsAdded = other.polygonsAdded;
        offset = other.offset;
        minSizePolygon = other.minSizePolygon;
    }
    return *this;
}

void MPolygonRaster::Setup(TArray<TArray<int>> &quads){
    shape = quads;
    UpdateFreePositions();
}

void MPolygonRaster::SetupAsChild(TArray<std::pair<int, int>> &positions, std::pair<int, int> &offsetIn){
    
    //min pos needed here: transform polygon data to origin!
    std::pair<int, int> minPosFound = MinPos(positions);
    // transform to 0,0
    minPosFound.first *= -1; //inverse translation
    minPosFound.second *= -1;
    MovePositions(minPosFound, positions);

    std::pair<int, int> maxPosFound = MaxPos(positions);
    offset = offsetIn;

    /*CoreMathDebugHelper::logMessage(
        FString::Printf(TEXT("Child Max Bound %d %d"), maxPosFound.first, maxPosFound.second)
    );*/
    FillShapeFromEmptySetupMarkLocked(maxPosFound.first, maxPosFound.second);
    
    markFree(positions);
    UpdateFreePositions();
    //Log("MPolygonRaster Setup As Child");
}

void MPolygonRaster::FillShapeFromEmptySetupMarkLocked(int i, int j){
    shape.Empty();
    for (int iCurrent = 0; iCurrent < i + 1; iCurrent++)
    {
        TArray<int> array;
        array.SetNum(j+1);
        for (int x = 0; x < array.Num(); x++){
            array[x] = lockedIndex;
        }
        shape.Add(array);
    }
}



std::pair<int, int> MPolygonRaster::MaxPos(TArray<std::pair<int, int>> &positions){
    std::pair<int,int> maxPos;
    if(positions.Num() > 0){
        maxPos = positions[0];
        for (int i = 1; i < positions.Num(); i++)
        {
            std::pair<int, int> &current = positions[i];
            maxPos.first = std::max(maxPos.first, current.first);
            maxPos.second = std::max(maxPos.second, current.second);
        }
    }
    return maxPos;
}

std::pair<int, int> MPolygonRaster::MinPos(TArray<std::pair<int, int>> &positions){
    std::pair<int, int> minPos;
    if(positions.Num() > 0){
        minPos = positions[0];
        for (int i = 1; i < positions.Num(); i++)
        {
            std::pair<int, int> &current = positions[i];
            minPos.first = std::min(minPos.first, current.first);
            minPos.second = std::min(minPos.second, current.second);
        }
    }
    return minPos;
}




TArray<std::pair<int, int>> MPolygonRaster::FreePositions() const {
    return freePositionsCache;
}

void MPolygonRaster::UpdateFreePositions(){
    freePositionsCache = FindFreePositions(shape);
}

TArray<std::pair<int, int>> MPolygonRaster::FindFreePositions(TArray<TArray<int>> &grid){
    TArray<std::pair<int, int>> outPositions;
    for (int i = 0; i < grid.Num(); i++)
    {
        TArray<int> &current = grid[i];
        for (int j = 0; j < current.Num(); j++){
            if(current[j] == freeIndex){
                outPositions.Add(std::pair<int, int>(i, j));
            }
        }
    }
    return outPositions;
}

TArray<std::pair<int, int>> MPolygonRaster::AllAsPositions(TArray<TArray<int>> &grid){
    TArray<std::pair<int, int>> outPositions;
    for (int i = 0; i < grid.Num(); i++)
    {
        TArray<int> &current = grid[i];
        for (int j = 0; j < current.Num(); j++){
            outPositions.Add(std::pair<int, int>(i, j));
        }
    }
    return outPositions;
}




void MPolygonRaster::markFree(TArray<std::pair<int, int>> &positions){
    for (int i = 0; i < positions.Num(); i++){
        markFree(positions[i]);
    }
}
void MPolygonRaster::markFree(std::pair<int, int> &index){
    markFree(index.first, index.second);
}

void MPolygonRaster::markFree(int i, int j){
    mark(i, j, freeIndex);
}


void MPolygonRaster::markLocked(TArray<std::pair<int,int>> &positions){
    for (int i = 0; i < positions.Num(); i++){
        markLocked(positions[i]);
    }
}

void MPolygonRaster::markLocked(std::pair<int,int> &index){
    markLocked(index.first, index.second);
}

void MPolygonRaster::markLocked(int i, int j){
    mark(i, j, polygonIndex);
}

void MPolygonRaster::mark(int i, int j, int someNum){
    mark(i, j, someNum, shape);
}

void MPolygonRaster::mark(int i, int j, int someNum, TArray<TArray<int>> &map){
    if(i >= 0 && i < map.Num()){
        TArray<int> &current = map[i];
        if(j >= 0 && j < current.Num()){
            current[j] = someNum; // lockedIndex; //lock = 1, free = 0
        }
    }
}






bool MPolygonRaster::isFree(std::pair<int,int> &index){
    return isFree(index.first, index.second);
}
bool MPolygonRaster::isFree(int i, int j){
    return isFree(i, j, shape);
}

bool MPolygonRaster::isFree(int i, int j, TArray<TArray<int>> &map){
    if(i >= 0 && i < map.Num()){
        TArray<int> &current = map[i];
        if(j >= 0 && j < current.Num()){
            return current[j] == freeIndex; //lock = 1, free = 0
        }
    }
    return false;
}

#include "CoreMath/algorithm/PolygonFitRasterized/MPolygonRasterizer.h"
void MPolygonRaster::TryFitAll(TArray<MPolygon> &polygons){
    if(freePositionsCache.Num() <= 0){
        return;
    }

    //sort smaller first
    if(polygons.Num() > 1){
        polygons.Sort([](const MPolygon& A, const MPolygon& B) {
            const FBoundingBox2D &boundA = A.boundingBox();
            const FBoundingBox2D &boundB = B.boundingBox();

            return boundA.size() < boundB.size();

            //return A > B; // true = A kommt vor B
        });
    }
    



    for (int i = 0; i < polygons.Num(); i++){
        bool result = Fit(polygons[i]);
    }
}
bool MPolygonRaster::Fit(MPolygon &polygon){
    MPolygonRasterizer rasterizer;
    MPolygonRaster output = rasterizer.MakeRasterized(polygon);

    bool bFitMinimal = true;
    if(bFitMinimal){
        return MinimalFit(output);
    }
    return Fit(output);
}

bool MPolygonRaster::Fit(MPolygonRaster &other){
    for (int i = 0; i < freePositionsCache.Num(); i++){
        std::pair<int, int> &currentPosition = freePositionsCache[i];
        if(FitImmidiate(currentPosition, other)){
            return true;
        }
    }
    return false;
}


void MPolygonRaster::SetMinSizePolygon(int x, int y){
    x = std::max(1, std::abs(x));
    y = std::max(1, std::abs(y));
    minSizePolygon.first = x;
    minSizePolygon.first = y;
}

bool MPolygonRaster::MinimalFit(MPolygonRaster &other){
    //int FindMin(TArary<MPolygonRasterFitCache> &array);
    TArray<MPolygonRasterFitCache> cachedOptions;

    for (int i = 0; i < freePositionsCache.Num(); i++)
    {   
        std::pair<int, int> &currentPosition = freePositionsCache[i];
        //rotation check
        int rotationSteps = 4;
        int rotationStepFraction = 360 / rotationSteps;
        for (int rNominator = 0; rNominator < rotationSteps; rNominator++){
            int rotationDregree = rNominator * rotationStepFraction;

            MPolygonRaster rotated = other.Rotate(rotationDregree); //const.

            MPolygonRasterFitCache cacheNew;
            //std::pair<int,int>& index, MPolygonRaster & other, MPolygonRasterFitCache & outCache
            if(FitTest(currentPosition, rotated, cacheNew)){
                cachedOptions.Add(cacheNew);
            }
        }

        /*MPolygonRasterFitCache cacheNew;
        std::pair<int, int> &currentPosition = freePositionsCache[i];
        if(FitTest(currentPosition, other, cacheNew)){
            cachedOptions.Add(cacheNew);
        }*/
    }
    if(cachedOptions.Num() > 0){
        int minIndex = MPolygonRasterFitCache::FindMin(cachedOptions);

        //fit at this pos
        if(minIndex >= 0 && minIndex < cachedOptions.Num()){
            MPolygonRasterFitCache &cache = cachedOptions[minIndex];
            std::pair<int, int> &pos = cache.GetPosition();
            MPolygonRaster &polygonCached = cache.GetMPolygonRaster();

            return FitImmidiate(pos, polygonCached);
            // return FitImmidiate(pos, other); //double looks up array. Stupid as hell.
        }
    }

    return false;



}

bool MPolygonRaster::FitTest(
    std::pair<int,int> &index, 
    MPolygonRaster &other, 
    MPolygonRasterFitCache &outCache //to be filled
){
    if(isFree(index)){
        TArray<std::pair<int, int>> movedPositions = other.FreePositions();
        MovePositions(index, movedPositions); //transform!

        TArray<TArray<int>> shapeCopy = shape;
        for (int i = 0; i < movedPositions.Num(); i++){
            std::pair<int, int> &current = movedPositions[i];
            if(!isFree(current)){
                return false;
            }
            //mark in copy buffer, to count unused space
            mark(current.first, current.second, 1, shapeCopy);
        }

        int gaps = GapsBelowMinSizePolygon(shapeCopy);
        outCache.Setup(gaps, index, other); //copy polygon data for adding later as valid
        return true;
    }
    return false;
}


int MPolygonRaster::GapsBelowMinSizePolygon(TArray<TArray<int>> &map){
    if(minSizePolygon.first > 0 && minSizePolygon.second > 0){

        int gaps = 0;
        for (int i = 0; i < map.Num(); i++)
        {
            TArray<int> &current = map[i];
            for (int j = 0; j < current.Num(); j++){
                //create sub search if free
                if(isFree(i,j, map)){
                    std::pair<int, int> pos(i, j);
                    if (HasGapBelowOrEqualMinSizePolygon(map, pos, minSizePolygon)){
                        gaps++;
                        i += minSizePolygon.first; //or second, min(f,s) --> ?
                    }
                }
            }
        }
    }
    return false;
}

bool MPolygonRaster::HasGapBelowOrEqualMinSizePolygon(
    TArray<TArray<int>> &map,
    std::pair<int,int> &position,
    std::pair<int,int> &minGapSize
){
    bool markFound = false;
    for (int i = position.first; i < map.Num(); i++)
    {
        TArray<int> &current = map[i];
        for (int j = position.second; j < current.Num(); j++){
            int deltaI = i - position.first;
            int deltaJ = j - position.second;

            //pos found in range which is not free: min gap found
            if(!isFree(i,j, map)){
                if(deltaI <= minGapSize.first && deltaJ <= minGapSize.second){
                    return true;
                }
                if(deltaJ <= minGapSize.first && deltaI <= minGapSize.second){
                    return true;
                }
            }

            //early quit if none found
            if(deltaI > minGapSize.first && deltaJ > minGapSize.second){
                return false;
            }
            if(deltaJ > minGapSize.first && deltaI > minGapSize.second){
                return false;
            }
        }
    }
    return false; //?
}

bool MPolygonRaster::FitImmidiate(std::pair<int,int> &index, MPolygonRaster &other){
    if(isFree(index)){
        TArray<std::pair<int, int>> noCollideLocal = other.FreePositions();
        MovePositions(index, noCollideLocal);
        for (int i = 0; i < noCollideLocal.Num(); i++){
            std::pair<int, int> &current = noCollideLocal[i];
            if(!isFree(current)){
                return false;
            }
        }
        //all free: add.
        markLocked(noCollideLocal);
        polygonIndex++; //for debug print: new numbers

        TArray<std::pair<int, int>> local = other.FreePositions(); //copy no offset
        AddPolygonToAddedTracker(local, index);

        UpdateFreePositions(); //would not be nesecarry, creates overhead i dont care.
        return true;
    }
    return false;
}

void MPolygonRaster::MovePositions(std::pair<int,int> &index, TArray<std::pair<int, int>> &local){
    for (int i = 0; i < local.Num(); i++){
        std::pair<int, int> &current = local[i];
        current.first += index.first;
        current.second += index.second;
    }
}


void MPolygonRaster::AddPolygonToAddedTracker(
    TArray<std::pair<int,int>> &positions,
    std::pair<int,int> &index
){
    MPolygonRaster rasternew;
    rasternew.SetupAsChild(positions, index);

    polygonsAdded.Add(rasternew);
}

// log to console
void MPolygonRaster::Log(){
    FString msg = TEXT("default log");
    Log(msg);
}

void MPolygonRaster::Log(FString prefix){
    CoreMathDebugHelper::logMessage("MPolygonRaster Print Polygon Start");

    for (int i = 0; i < shape.Num(); i++){
        FString line = FString::Printf(TEXT("%s MPolygonRaster line:%d \t ["),*prefix, i);
        TArray<int> &currentLine = shape[i];
        for (int j = 0; j < currentLine.Num(); j++){
            int num = currentLine[j];
            line += FString::Printf(TEXT("%d \t"), num);
        }
        line += TEXT("]");
        CoreMathDebugHelper::logMessage(line);
    }
    CoreMathDebugHelper::logMessage("MPolygonRaster Print Polygon End");
}










/// --- create draw mesh data ---
TArray<FVector2D> MPolygonRaster::createQuadMeshData(int step){
    TArray<FVector2D> quadShapedMeshData;
    for (int i = 0; i < shape.Num(); i++){
        TArray<int> &currentArray = shape[i];
        for (int j = 0; j < currentArray.Num(); j++){
            /*
            j
            |
            1  2

            0  3 -->i
            */
            if(isFree(i, j)){
                FVector2D v0(i * step, j * step);
                FVector2D v1(i * step, (j+1) * step);
                FVector2D v2((i+1) * step, (j+1) * step);
                FVector2D v3((i+1) * step, j * step);
                quadShapedMeshData.Add(v0);
                quadShapedMeshData.Add(v1);
                quadShapedMeshData.Add(v2);
                quadShapedMeshData.Add(v3);
            }

        }
    }

    //add offset
    FVector2D offsetAsVector(offset.first, offset.second);
    offsetAsVector *= step;
    for (int i = 0; i < quadShapedMeshData.Num(); i++)
    {
        FVector2D &current = quadShapedMeshData[i];
        current += offsetAsVector;
    }

    return quadShapedMeshData;
}


TArray<TArray<FVector2D>> MPolygonRaster::createQuadMeshDataChildren(int step){
    TArray<TArray<FVector2D>> outArray;
    if(polygonsAdded.Num() > 0){
        outArray.SetNum(polygonsAdded.Num());
        for (int i = 0; i < polygonsAdded.Num(); i++)
        {
            MPolygonRaster &currentPolygon = polygonsAdded[i];
            TArray<FVector2D> &current = outArray[i];
            current = currentPolygon.createQuadMeshData(step);
        }
    }
    return outArray;
}








//// experimental
#include "CoreMath/Matrix/2D/MMatrix2D.h"
MPolygonRaster MPolygonRaster::Rotate(int degree) const {
    //direkt rotate test?
    //dann raster komplett cachen?
    if(degree % 360 == 0){
        return *this;
    }

    //make position modded
    TArray<std::pair<int, int>> positions = FreePositions();
    MMatrix2D mat;
    mat.RadAdd(MMatrix2D::degToRadian(degree));
    for (int i = 0; i < positions.Num(); i++){
        std::pair<int, int> &current = positions[i];
        FVector2D asVec(current.first, current.second);
        asVec = mat * asVec;
        //back to int
        current.first = int(asVec.X);
        current.second = int(asVec.Y);
    }
    std::pair<int, int> pivotNone(0, 0);
    MPolygonRaster rasterRotated;
    rasterRotated.SetupAsChild(positions, pivotNone);

    //rasterRotated.Log("MPolygonRaster Rotated Polygon:");

    return rasterRotated;
}

void MPolygonRaster::Transpose(TArray<TArray<int>> &map){
    if(map.Num() <= 0){
        return;
    }

    TArray<TArray<int>> transposed;
    int iLimit = map.Num();
    int jLimit = map[0].Num();
    transposed.SetNum(jLimit);
    for (int i = 0; i < transposed.Num(); i++){
        transposed[i].SetNum(iLimit);
    }

    for (int i = 0; i < iLimit; i++)
    {
        for (int j = 0; j < jLimit; j++)
        {
            transposed[j][i] = map[i][j];
        }
    }
    shape = transposed;
}
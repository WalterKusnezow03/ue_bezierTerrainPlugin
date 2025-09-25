#pragma once 

#include "CoreMinimal.h"
#include "CoreMath/algorithm/PolygonFit/polygon/MPolygon.h"
#include <utility>

class MPolygonRasterFitCache;

//polygon as raster
class COREMATH_API MPolygonRaster {
private:
    bool bLogEnabled = false;

public:
    MPolygonRaster();
    ~MPolygonRaster();

    MPolygonRaster(const MPolygonRaster &other);
    MPolygonRaster &operator=(const MPolygonRaster &other);

    void Setup(TArray<TArray<int>> &quads);
    void SetupAsChild(TArray<std::pair<int, int>> &quads, std::pair<int,int> &index);

    void SetMinSizePolygon(int x, int y);

    void UpdateFreePositions();

    void TryFitAll(TArray<MPolygon> &polygons); //sorting modified!
    bool Fit(MPolygon &polygon);

    bool Fit(MPolygonRaster &other);
    bool FitImmidiate(std::pair<int,int> &index, MPolygonRaster &other);
    bool MinimalFit(MPolygonRaster &other);


    /// @brief for drawing
    TArray<FVector2D> createQuadMeshData(int step);

    void Log();
    void Log(FString prefix);

    TArray<TArray<FVector2D>> createQuadMeshDataChildren(int step);

protected:
    TArray<std::pair<int, int>> FreePositions() const;
    void FillShapeFromEmptySetupMarkLocked(int i, int j);

    std::pair<int, int> MinPos(TArray<std::pair<int, int>> &positions);
    std::pair<int, int> MaxPos(TArray<std::pair<int, int>> &positions);

    TArray<std::pair<int, int>> FindFreePositions(TArray<TArray<int>> &grid);


    TArray<std::pair<int, int>> AllAsPositions(TArray<TArray<int>> &grid);

private:
    void markLocked(TArray<std::pair<int, int>> &positions);
    void markLocked(std::pair<int, int> &index);
    void markLocked(int i, int j);

    void markFree(TArray<std::pair<int, int>> &positions);
    void markFree(std::pair<int, int> &index);
    void markFree(int i, int j);

    void mark(int i, int j, int someNum);
    void mark(int i, int j, int someNum, TArray<TArray<int>> &map);

    bool isFree(std::pair<int, int> &index);
    bool isFree(int i, int j);
    bool isFree(int i, int j, TArray<TArray<int>> &map);

    void MovePositions(std::pair<int, int> &index, TArray<std::pair<int, int>> &local);

    bool FitTest(
        std::pair<int, int> &index,
        MPolygonRaster &other,
        MPolygonRasterFitCache &outCache // to be filled
    );

    int freeIndex = 0;
    int lockedIndex = 1;
    int polygonIndex = 2; //start at 2
    TArray<TArray<int>> shape;
    TArray<std::pair<int, int>> freePositionsCache;

    //cache polygons added for drawing / meshdata generation
    void AddPolygonToAddedTracker(TArray<std::pair<int,int>> &polygonRaster, std::pair<int,int> &index);
    TArray<MPolygonRaster> polygonsAdded;



    std::pair<int, int> offset;


    /// @brief if a gap smaller than this min size is found in the grid
    ///its. Bad.
    std::pair<int, int> minSizePolygon;

    int GapsBelowMinSizePolygon(TArray<TArray<int>> &map);
    bool HasGapBelowOrEqualMinSizePolygon(
        TArray<TArray<int>> &map,
        std::pair<int, int> &position,
        std::pair<int, int> &minGapSize);

    void Transpose(TArray<TArray<int>> &map);
    MPolygonRaster Rotate(int degree) const;
};
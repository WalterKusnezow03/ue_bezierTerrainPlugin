#pragma once 

#include "CoreMinimal.h"
#include "CoreMath/algorithm/PolygonFitRasterized/MPolygonRaster.h"

class COREMATH_API MPolygonRasterFitCache {

public:
    MPolygonRasterFitCache();
    ~MPolygonRasterFitCache();
    MPolygonRasterFitCache(const MPolygonRasterFitCache &other);
    MPolygonRasterFitCache &operator=(const MPolygonRasterFitCache &other);

    void Setup(int leftGaps, std::pair<int, int> &pos, MPolygonRaster &polygon);

    int LeftSpace();
    std::pair<int, int> &GetPosition();
    MPolygonRaster &GetMPolygonRaster();

    static int FindMin(TArray<MPolygonRasterFitCache> &array);

private:
    int leftSpace = 0;
    std::pair<int, int> position;
    MPolygonRaster cachedPolygonRaster;


};
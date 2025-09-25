#pragma once

#include "CoreMinimal.h"
#include "CoreMath/algorithm/PolygonFit/polygon/MPolygon.h"
#include "CoreMath/algorithm/PolygonFitRasterized/MPolygonRaster.h"
#include <utility>


class COREMATH_API MPolygonRasterizer {

public:
    MPolygonRasterizer();
    ~MPolygonRasterizer();

    void Rasterize(MPolygon &other, TArray<TArray<FVector2D>> &outputQuads);

    MPolygonRaster MakeRasterized(MPolygon &other);


    //static int MakeModX(FVector2D &pos, int modValue, int sign);
    static int MakeModY(FVector2D &pos, int modValue, int sign);

private:
    int sizeQuad = 10;

    void GetVerticalRange(
        FVector2D &v0,
        FVector2D &v1,
        MPolygon &other,
        TArray<std::pair<FVector2D, FVector2D>> &pairs
    );

    void AddValidQuads(
        std::pair<FVector2D, FVector2D> &edge,
        TArray<FVector2D> &quads
    );

    void AddValidQuads(
        std::pair<FVector2D, FVector2D> &edge,
        std::pair<FVector2D, FVector2D> &bounds,
        TArray<int> &quads
    );

    

    void Log(TArray<std::pair<FVector2D, FVector2D>> &pairs);
};
#include "MPolygonRasterizer.h"
#include "CoreMath/Debug/CoreMathDebugHelper.h"


MPolygonRasterizer::MPolygonRasterizer(){

}

MPolygonRasterizer::~MPolygonRasterizer(){

}


void MPolygonRasterizer::Rasterize(MPolygon &other, TArray<TArray<FVector2D>> &outputQuads){
    int boundsIncrease = 20;
    const FBoundingBox2D &bounds = other.boundingBox();
    FVector2D minXminY = bounds.min();
    FVector2D maxXmaxY = bounds.max();

    CoreMathDebugHelper::logMessage(
        FString::Printf(TEXT(
            "MPolygonRasterizer::Rasterize Bound (%s %s)"
        ),
        *minXminY.ToString(),
        *maxXmaxY.ToString()
        )
    );

    FVector2D yStep(0, maxXmaxY.Y + boundsIncrease);
    TArray<std::pair<FVector2D, FVector2D>> pairs;

    int steps = bounds.sizeX() / sizeQuad;
    FVector2D pivot = minXminY - FVector2D(0, boundsIncrease);
    for (int i = 0; i < steps; i++)
    {
        pivot += FVector2D(sizeQuad, 0);
        FVector2D upper = pivot + yStep;
        GetVerticalRange(
            pivot,
            upper,
            other,
            pairs
        );
    }

    //add each edge
    for (int i = 0; i < pairs.Num(); i++){
        TArray<FVector2D> singleEdge;
        AddValidQuads(
            pairs[i],
            singleEdge
        );
        outputQuads.Add(singleEdge);
    }


    //log(?)
    Log(pairs);
}


MPolygonRaster MPolygonRasterizer::MakeRasterized(MPolygon &other){
    int boundsIncrease = 20;
    const FBoundingBox2D &bounds = other.boundingBox();
    FVector2D minXminY = bounds.min();
    FVector2D maxXmaxY = bounds.max();

    CoreMathDebugHelper::logMessage(
        FString::Printf(TEXT(
            "MPolygonRasterizer::Rasterize Bound (%s %s)"
        ),
        *minXminY.ToString(),
        *maxXmaxY.ToString()
        )
    );

    FVector2D yStep(0, maxXmaxY.Y + boundsIncrease);
    TArray<std::pair<FVector2D, FVector2D>> pairs;

    int steps = bounds.sizeX() / sizeQuad;
    FVector2D pivot = minXminY - FVector2D(0, boundsIncrease);
    for (int i = 0; i < steps; i++)
    {
        pivot += FVector2D(sizeQuad, 0);
        FVector2D upper = pivot + yStep;
        GetVerticalRange(
            pivot,
            upper,
            other,
            pairs
        );
    }

    //add each edge
    TArray<TArray<int>> outputQuads;
    std::pair<FVector2D, FVector2D> boundsPair(minXminY, maxXmaxY);
    for (int i = 0; i < pairs.Num(); i++)
    {
        TArray<int> singleEdge;
        AddValidQuads(
            pairs[i],
            boundsPair,
            singleEdge
        );
        outputQuads.Add(singleEdge);
    }

    MPolygonRaster outRaster;
    outRaster.Setup(outputQuads); //align to pivot 0,0
    return outRaster;
}

//ok
void MPolygonRasterizer::GetVerticalRange(
    FVector2D &v0,
    FVector2D &v1,
    MPolygon &other,
    TArray<std::pair<FVector2D, FVector2D>> &pairs
){
    /*
    bool DoesIntersect(
        const FVector2D &aWorld, 
        const FVector2D &bWorld,
        TArray<FVector2D> &intersectionsOut //is Cleared
    );
    */
    TArray<FVector2D> intersections;
    if(other.DoesIntersect(v0,v1,intersections)){
        for (int i = 0; i + 1 < intersections.Num(); i += 2){
            FVector2D &lower = intersections[i];
            FVector2D &higher = intersections[i+1];

            FVector2D &lowerSorted = lower.Y < higher.Y ? lower : higher;
            FVector2D &higherSorted = lower.Y > higher.Y ? lower : higher;

            std::pair<FVector2D, FVector2D> pair(lowerSorted, higherSorted);
            pairs.Add(pair);
        }
    }else{
        //add previous test
        if(pairs.Num() > 0){
            std::pair<FVector2D, FVector2D> copy = pairs.Last();
            copy.first.X = v0.X;
            copy.second.X = v0.X;
            pairs.Add(copy);
        }
    }
}




void MPolygonRasterizer::AddValidQuads(
    std::pair<FVector2D, FVector2D> &edge,
    TArray<FVector2D> &quads
){
    if(sizeQuad == 0){
        return;
    }

    //check parallell bounds lower and add the quads (sizeQuad step up)
    int lowerYasInt = int(edge.first.Y);
    int overFlow = lowerYasInt % sizeQuad; //mod
    int toNext = sizeQuad - overFlow;
    lowerYasInt += toNext;

    int higherYasInt = int(edge.second.Y);
    overFlow = higherYasInt % sizeQuad;//mod
    int toPrev = sizeQuad - overFlow;
    higherYasInt -= toPrev;

    for (int i = lowerYasInt; i < higherYasInt; i+=sizeQuad){
        FVector2D result(edge.first.X, i);
        quads.Add(result);
    }
}



void MPolygonRasterizer::AddValidQuads(
    std::pair<FVector2D, FVector2D> &edge,
    std::pair<FVector2D, FVector2D> &bounds,
    TArray<int> &quads
){
    if(sizeQuad == 0){
        return;
    }

    //check parallell bounds lower and add the quads (sizeQuad step up)
    int lowerYasInt = MakeModY(edge.first, sizeQuad, 1);
    int higherYasInt = MakeModY(edge.second, sizeQuad, -1);

    //+1 by design.
    int boundYlower = MakeModY(bounds.second, sizeQuad, -1);
    int boundYhigher = MakeModY(bounds.second, sizeQuad, 1);

    for (int i = boundYlower; i < lowerYasInt; i+=sizeQuad){
        quads.Add(1);
    }

    for (int i = lowerYasInt; i < higherYasInt; i+=sizeQuad){
        quads.Add(0);
    }

    for (int i = higherYasInt; i <= boundYhigher; i+=sizeQuad){
        quads.Add(1);
    }
}

int MPolygonRasterizer::MakeModY(FVector2D &pos, int modValue, int sign){
    int Y_asInt = int(pos.Y);
    int overFlow = Y_asInt % modValue;
    int toNext = modValue - overFlow;
    Y_asInt += sign * toNext;
    return Y_asInt;
}

//ok
void MPolygonRasterizer::Log(TArray<std::pair<FVector2D, FVector2D>> &pairs){
    CoreMathDebugHelper::logMessage(FString::Printf(TEXT("MPolygonRasterizer intersections log %d"), pairs.Num()));
    for (int i = 0; i < pairs.Num(); i++){
        std::pair<FVector2D, FVector2D> &current = pairs[i];
        FVector2D &a = current.first;
        FVector2D &b = current.second;

        FString message = FString::Printf(
            TEXT("MPolygonRasterizer: pair: %s %s"),
            *a.ToString(), *b.ToString()
        );
        CoreMathDebugHelper::logMessage(message);
    }
}
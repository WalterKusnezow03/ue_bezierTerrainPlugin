#pragma once

#include "CoreMinimal.h"

class TERRAINPLUGIN_API HeightExtractionData{
public:
    HeightExtractionData(FVector &VertexWorldSpace, FVector &positionTargetWorld);
    ~HeightExtractionData();

    HeightExtractionData(const HeightExtractionData &other);
    HeightExtractionData &operator=(const HeightExtractionData &other);

    void SetupDataAndWeighting(FVector &vertexWorldSpace);
    static float findHeight(
        TArray<HeightExtractionData> &array,
        float einheitsValue
    );
    FVector &vertexPositionToLookFor();

private:
    float height = 0.0f;
    float distanceFromTarget = 0.0f;

    FVector positionVertex;
    FVector positionTarget; //referenz position nach der gewichtet wird

    static constexpr bool debugLog = false;
};

#include "HeightExtractionData.h"

/**
 * height extraction container
 */
HeightExtractionData::HeightExtractionData(
    FVector &VertexWorldSpace, //look for in terrain chunk pos
    FVector &positionTargetWorld
){
    positionVertex = VertexWorldSpace;
    positionTarget = positionTargetWorld;
}
HeightExtractionData::~HeightExtractionData(){
    //none
}

HeightExtractionData::HeightExtractionData(const HeightExtractionData &other){
    *this = other;
}

HeightExtractionData &HeightExtractionData::operator=(
    const HeightExtractionData &other
){
    if(this != &other){
        height = other.height;
        distanceFromTarget = other.distanceFromTarget;
        positionVertex = other.positionVertex;
        positionTarget = other.positionTarget;
    }
    return *this;
}

/// @brief vertex must be in world space!
/// @param vertexWorldSpace 
void HeightExtractionData::SetupDataAndWeighting(
    FVector &vertexWorldSpace
){
    height = vertexWorldSpace.Z;

    FVector2D a(vertexWorldSpace.X, vertexWorldSpace.Y);
    FVector2D b(positionTarget.X, positionTarget.Y);
    distanceFromTarget = FVector2D::Distance(a, b);

    if(debugLog){
        DebugHelper::showScreenMessage(
            "terrain vertex distance ",
            (float) distanceFromTarget
        );
    }
}

FVector &HeightExtractionData::vertexPositionToLookFor(){
    return positionVertex;
}

float HeightExtractionData::findHeight(
    TArray<HeightExtractionData> &array,
    float EinheitsValue
){
    float heightOutput = 0.0f;
    float weightSum = 0.0f;
    for (int i = 0; i < array.Num(); i++)
    {
        HeightExtractionData &current = array[i];
        float percent = current.distanceFromTarget / EinheitsValue; //a / 1
    
        float invert = 1.0f - percent;
        invert = std::min(1.0f, invert);
        invert = std::max(0.0f, invert);

        float heightFraction = invert * current.height;

        if(debugLog){
            FString s = FString::Printf(TEXT("terrain vertex : (height %.2f) (scalar %.2f) (distance %.2f)"),
                                    (float)current.height, (float)invert, current.distanceFromTarget);
            DebugHelper::showScreenMessage(s);
        }
        

        heightOutput += heightFraction;
        weightSum += invert;
    }

    if(true){
        heightOutput /= weightSum; //normalisieren ganz wichtig
    }
    

    return heightOutput;
}
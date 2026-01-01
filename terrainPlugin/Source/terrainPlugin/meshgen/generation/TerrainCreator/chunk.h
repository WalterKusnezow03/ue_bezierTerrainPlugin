#pragma once

#include "CoreMinimal.h"
#include "terrainHillSetup.h"
#include <set>
#include "terrainPlugin/meshgen/generation/helper/TerrainChunkSetup.h"
#include "GameCore/MeshGenBase/foliage/ETerrainType.h"
#include "GameCore/util/FVectorTouple.h"
#include "GameCore/util/TVector.h"

#include "GameCore/MeshGenBase/MeshData/BoundingBox/BoundingShapeXY.h"

/**
 * 
 */
class TERRAINPLUGIN_API chunk
{
public:
    chunk(int xPos, int yPos);
    ~chunk();

    TerrainChunkSetup makeSetupPackage();
    TerrainChunkSetup makeSetupPackage(
        chunk *top,
        chunk *right,
        chunk *topRight);

    bool NextWorldVertexAt(FVector &a, FVector &out);

    
    FVector position();
    FVector positionPivotBottomLeft();

    void addheightForAll(int value);
    void scaleheightForAll(float value);
    void setheightForAll(float value);
    void setheightForAllToAverage();
    void clampheightForAllUpperLimit(float value);
    void clampheightForAllUpperLimitByOwnAverageHeight();

    FVector2D getFirstXColumnAnchor(int xColumn);
    FVector2D getFirstYRowAnchor(int yRow);

    void plot(UWorld *world);

    void applyIndivualVertexIndexBased(
        int xIn,
        int yIn,
        float newHeight,
        bool override);

    std::vector<std::vector<FVector>> &readMap();

private:
    std::vector<std::vector<FVector>> &readAndMerge(
        chunk *top,
        chunk *right,
        chunk *topRight
    );

public:
    void Merge(
        chunk *top,
        chunk *right,
        chunk *topRight
    );


    bool xIsValid(int a);
    bool yIsValid(int a);

    void setTreesBlocked(bool b);
    bool createTrees();

    ETerrainType getTerrainType();
    void updateTerraintype(ETerrainType typeIn);

    float heightAverage();
    float maxHeight();
    float minHeight();

    void setWasCreatedTrue();
    bool wasAlreadyCreated();

    void updateTerrainTypeBySpecialHeights();

    void markCreateOutpostTrue();
    void markBuildingCreateTrueAndBlockTrees();

    void blockAreaForFoliage(
        FVector &v0,
        FVector &v1,
        FVector &v2,
        FVector &v3
    );
    void blockAreaForFoliage(FVector &a, FVector &b);
    void freePositionsForFoliageLocal(
        TArray<FVectorTouple> &outpositions);

private:
    void generateBoundingIndicesFromWorldSpace(
        FVector &a,
        FVector &b,
        int &minX,
        int &minY,
        int &maxX,
        int &maxY
    );

    bool indexFreeForFoliage(int i, int j);
    void lockPositionForAnyFoliage(int i, int j);
    void lockPositionForAnyFoliageIfTrue(int i, int j, bool flag);

    bool wasCreated = false;
    ETerrainType savedTerrainType = ETerrainType::ETropical;
    bool createOutpost = false;
    bool blockTrees = false;
    bool createBuilding = false;

    std::vector<std::vector<FVector>> innerMap;
    std::vector<std::vector<bool>> innerMapFreePositions;
    int x;
    int y;
    

    int clampInnerIndex(int a);

    FVector normalFor(int i, int j);

    int convertToInnerIndex(int value);
    int clampOuterYIndex(FVector2D &a);
    bool isInBounds(FVector &a);

    int xPositionInCm();
    int yPositionInCm();

    void convertPositionToInnerIndexClamped(
        FVector inpos,
        int &i,
        int &j);

    std::vector<FVector> readFirstXColumn();
    std::vector<FVector> readFirstYRow();
    FVector readBottomLeftCorner();

    std::vector<bool> readFirstXColumnFoliage();
    std::vector<bool> readFirstYRowFoliage();
    bool readBottomLeftCornerFoliage();
	



};
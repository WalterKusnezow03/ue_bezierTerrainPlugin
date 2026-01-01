#pragma once

#include "CoreMinimal.h"
#include "terrainPlugin/meshgen/generation/TerrainCreator/Road/RoadGrid/RoadQuad.h"
#include "terrainPlugin/meshgen/generation/TerrainCreator/Road/RoadGrid/RoadKey.h"
#include "terrainPlugin/meshgen/generation/TerrainCreator/Road/RoadGrid/RoadKeyEnum.h"
#include "terrainPlugin/meshgen/generation/bezier/extendedVersion/TBSplinePosition.h"

#include "terrainPlugin/main/worldCache/ChunkParserMap.h"
#include "terrainPlugin/meshgen/generation/TerrainCreator/TerrainMeshDataParser/ChunkParser.h"

#include "CoreMath/Matrix/MMatrix.h"

/// -- 
/// creates a road mesh from a grid, displacing vertecies
/// and then interpolates with bsplines.
/// will also store the closed polygons for building areas
/// --

class terrainCreator;

class TERRAINPLUGIN_API RoadMakerFromGrid {

public:
    RoadMakerFromGrid();
    ~RoadMakerFromGrid();

    ///create mesh grid with respect to step size and internal quad mesh for later extraction
    ///of polygons
    void CreateGrid(FVector2D size, float StepSize);

    void RandomizeGrid();

    // --- warp grid with different functions ---
    void WarpCirlceRandom();
    void WarpCirlceByDistanceAroundCenter(
        float maxDistance,
        float angle);
    void WarpCirlceByDistance(
        FVector &center,
        float maxDistance,
        float angle);

    // --- build --- (no warp allowed anymore)
    void Build(
        terrainCreator *creator, 
        float _einheitsValueForBsplineStepSize, 
        float roadWidth,
        ChunkParserMap &map
    ); //einheitsvalue: ditsance between nodes interpolated

    // -- todo --
    //for creation of buildings
    //TArray<MPolygon> PolygonsForBuildingFit();



protected:
    //raw mesh 
    TArray<TArray<FVector>> mesh;
    
    //helper properites for distortion

    /// @brief center of mesh after setup
    FVector centerSaved;

    /// @brief original property of size on setup
    FVector2D sizeSaved;
    float stepSizeSavedBetweenMeshNodes = 0.0f; // on setup stepsize
    void SaveOriginalProperties(
        FVector2D &size,
        float stepSize
    );
    FVector &CenterOfMesh();
    void UpdateCenterOfMesh();

    //private warp
    void MakeRotationAroundPosition(
        FVector &position,
        float angle,
        MMatrix &outMatrix
    );

    //builded mesh
    bool wasBuilded = false;
    TArray<TArray<RoadQuad>> buildedMeshQuads; //any shape, but originally the quads
    RoadQuad &GetQuadInBuildedMesh(std::pair<int, int> &posXY);
    RoadQuad &GetQuadInBuildedMesh(int i, int j);
    RoadQuad fallback;


    void ComputeAllYAxis(float _einheitsValue);
    void ComputeAllXAxis(float _einheitsValue);
    void ComputeYAxis(int index, float _einheitsValue);
    void ComputeXAxis(int index, float _einheitsValue);

    //builded sections unadded
    std::map<FRoadKey, TArray<FVectorBSplinePosition>> buildedSections;
    TArray<FVectorBSplinePosition> &GetSection(int index, ERoadKeyEnum key);
    TArray<FVectorBSplinePosition> &GetSection(FRoadKey &key);

    
    //as (x,y) pairs for copying axis section parts
    TArray<std::pair<int, int>> GetAllQuadShapedIndices();

    ///where the original array had its anchor (original grid), find that index
    //inside the passed array (0,1,2,3,x<-target,...) - finds: 4
    int IndexInArrayWhereOriginalAnchorIsIndex(
        int targetAnchor,
        TArray<FVectorBSplinePosition> &completeSection
    );

    TArray<FVectorBSplinePosition> CopySection(
        std::pair<int,int> &pos0,
        std::pair<int,int> &pos1
    );

    TArray<FVectorBSplinePosition> CopySection(
        ERoadKeyEnum axis,
        int axisIndex,
        int a,
        int b
    );
    TArray<FVectorBSplinePosition> CopySection(
        TArray<FVectorBSplinePosition> &array,
        int i0,
        int i1
    );

    void CopySectionTo(
        std::pair<int, int> &pos0,
        std::pair<int, int> &pos1,
        RoadQuad &quad
    );

    void Build(float detail);
    void BuildAllPolygons();
    void ApplyHeightOnRawMesh(terrainCreator *creator);
    void ValidateSize(FVector2D &size, float &stepSize);

    /// --- update terrain and chunk parsers. ---
    void LockTerrainFromGeneratedRoadQuads(terrainCreator *creator, float roadWidth);

    void ApplyTerrain2DIndexToRoadQuadsAndRemoveTerrainOffset(
        terrainCreator *creator
    );
    void ApplyTerrain2DIndexToRoadQuads(
        terrainCreator *creator);
    void RemoveTerrainOffsetFromRoadQuads(
        terrainCreator *creator
    );
    void ApplyHeightOnRoadQuads(terrainCreator *creator);
    void ApplyHeightForPostions(TArray<FVector> &array, terrainCreator *creator);

    void AddRoadQuadsToChunks(ChunkParserMap &map);
    void AddRoadQuadToChunks(
        ChunkParser &chunk,
        RoadQuad &quad
    );

    
};

#pragma once

#include "terrainPlugin/meshgen/generation/TerrainCreator/terrainConstants.h"
#include "terrainPlugin/meshgen/generation/bezierCurve.h"
#include "terrainPlugin/meshgen/generation/TerrainCreator/chunk.h"

class terrainCreator;
class TERRAINPLUGIN_API RoadMaker
{

public:
    RoadMaker();

    //--road--
	void createRoads(terrainCreator *ptr, UWorld *world, int chunks);
	

private:
    float getHeightFor(FVector2D &pos);

    void createRoads(
        MeshData &outmeshDataSurface,
        MeshData &outmeshDataSides,
        int count,
        int chunks
    );
    void createRoad(
        MeshData &outmeshDataSurface,
        MeshData &outmeshDataSides,
        int chunks
    );
    void processRoad(
        TVector<FVector2D> &curve,
        float roadWidth,
        MeshData &outmeshDataSurface,
        MeshData &outmeshDataSides,
        float _einheitsValue
    );
    FVector make3D(FVector2D &pos, float height);
    void lockQuadsFromParalellArrayLines(
        TArray<FVector> &line0,
        TArray<FVector> &line1
    );


    terrainCreator *terrainCreatorPtr = nullptr;
    AcustomMeshActor *getNewMeshActor();
};
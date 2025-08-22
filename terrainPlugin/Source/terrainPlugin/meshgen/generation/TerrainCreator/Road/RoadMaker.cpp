#include "RoadMaker.h"
#include "terrainPlugin/meshgen/generation/TerrainCreator/terrainCreator.h"
#include "GameCore/EntityGC/EntityManagerBase.h"


RoadMaker::RoadMaker(){
    terrainCreatorPtr = nullptr;
}

float RoadMaker::getHeightFor(FVector2D &pos){
    if(terrainCreatorPtr){
        return terrainCreatorPtr->getHeightFor(pos);
    }
    return 0.0f;
}


void RoadMaker::createRoads(terrainCreator *ptr, UWorld *world, int chunks){

    terrainCreatorPtr = ptr;
    if (world && terrainCreatorPtr)
    {
        AcustomMeshActor *currentActor = terrainCreatorPtr->getNewMeshActor();
        if (currentActor == nullptr)
        {
            return;
        }
        currentActor->disableDistanceListening();
        MeshData &meshdataSurface = currentActor->findMeshDataReference(
            materialEnum::stoneMaterial,
            true //has raycast
        );
        MeshData &meshdataSides = currentActor->findMeshDataReference(
            materialEnum::beigeStoneMaterial,
            true //has raycast
        );
        createRoads(meshdataSurface, meshdataSides, 2, chunks);
        meshdataSurface.calculateNormals();
        meshdataSides.calculateNormals();
        currentActor->ReloadMeshAndApplyAllMaterials();
    }
}

void RoadMaker::createRoads(
    MeshData &outmeshDataSurface,
    MeshData &outmeshDataSides,
    int count,
    int chunks
){
    for(int i = 0; i < count; i++){
        createRoad(outmeshDataSurface, outmeshDataSides, chunks);
    }
}


void RoadMaker::createRoad(
    MeshData &outmeshDataSurface,
    MeshData &outmeshDataSides,
    int chunks
){
    
    int scalePerChunk = terrainConstants::CHUNKSIZE * terrainConstants::ONEMETER;
    int limitall = scalePerChunk * chunks;
	
    //eine strasse erstreckt sich immer über die ganze karte z.b.


    bezierCurve curve;
    TVector<FVector2D> output;
    float _einheitsValue = terrainConstants::ONEMETER;

    if(true){
        /*
        FVector2D &startingPoint,
        TVector<FVector2D> &output,
        float _einheitsValue,
        float distanceBetweenAnchorsOnXAxisMin,
        float distanceBetweenAnchorsOnXAxisMax,
        float distanceBetweenAnchorsYRange,
        float max_xy_coordinate
        */

        FVector2D startingPoint;
        float distanceBetweenAnchorsOnXAxisMin = scalePerChunk * 1.0f;
        float distanceBetweenAnchorsOnXAxisMax = scalePerChunk * 4.0f;
        float distanceBetweenAnchorsYRange = scalePerChunk * 2.0f;
        float max_xy_coordinate = limitall;

        curve.createNewRandomCurve(
            startingPoint,
            output,
            _einheitsValue,
            distanceBetweenAnchorsOnXAxisMin,
            distanceBetweenAnchorsOnXAxisMax,
            distanceBetweenAnchorsYRange,
            max_xy_coordinate
        );
    }

    float roadWidth = terrainConstants::ONEMETER * 5.0f;
    processRoad(
        output, 
        roadWidth, 
        outmeshDataSurface, 
        outmeshDataSides, 
        _einheitsValue
    );

    //assign data to mesh actor when done.

}

void RoadMaker::processRoad(
    TVector<FVector2D> &curve,
    float roadWidth,
    MeshData &outmeshDataSurface,
    MeshData &outmeshDataSides,
    float _einheitsValue
){
    TArray<FVector> line1;
    TArray<FVector> line2;
    line1.SetNum(curve.size());
    line2.SetNum(curve.size());


    roadWidth = std::abs(roadWidth);
    float halfWidht = roadWidth / 2.0f;
    for(int i = 1; i < curve.size(); i++){
        FVector2D &prev = curve[i-1];
        FVector2D &current = curve[i];

        //an normla extenden und beide seiten vertecies suchen, higher one
        FVector2D AB = current - prev; //AB = B - A;
        FVector2D normal(AB.Y, -AB.X); 
        normal = normal.GetSafeNormal();

        FVector2D side0_2d = prev + normal * halfWidht;
        FVector2D side1_2d = prev + -1.0f * normal * halfWidht;

        float side0Heigth = getHeightFor(side0_2d);
        float side1Heigth = getHeightFor(side1_2d);
        float maxHeight = std::max(side0Heigth, side1Heigth);

        FVector side0 = make3D(side0_2d, maxHeight);
        FVector side1 = make3D(side1_2d, maxHeight);

        line1[i] = side0;
        line2[i] = side1;

    }


    //make road look less clunky
    bezierCurve bezierCurveMaker;
    int skipIndicesForSmooth = 10;
    bezierCurveMaker.afterSmoothHeight(
        line1, //TArray<FVector> &curve,
        _einheitsValue,
        skipIndicesForSmooth//int anchorSkipPerStep
    );
    bezierCurveMaker.afterSmoothHeight(
        line2, 
        _einheitsValue,
        skipIndicesForSmooth
    );



    //generate buffer:
    /*
    1->2
    |  |
    0<-3
    */
    outmeshDataSurface.appendParalellLinesClosedAsQuads(line1, line2);

    //TESTING NEEDED
    //extend sides to bottom
    TArray<FVector> line1Bottom = line1;
    TArray<FVector> line2Bottom = line2;
    for (int i = 0; i < line1Bottom.Num(); i++){
        FVector &current = line1Bottom[i];
        current += FVector(0, 0, -200);
    }
    for (int i = 0; i < line2Bottom.Num(); i++){
        FVector &current = line2Bottom[i];
        current += FVector(0, 0, -200);
    }
    outmeshDataSides.appendParalellLinesClosedAsQuads(line1Bottom, line1);
    outmeshDataSides.appendParalellLinesClosedAsQuads(line2, line2Bottom);




    // blocks terrain
    lockQuadsFromParalellArrayLines(line1, line2);
}

FVector RoadMaker::make3D(FVector2D &pos, float height){
    return FVector(
        pos.X,
        pos.Y,
        height
    );
}



void RoadMaker::lockQuadsFromParalellArrayLines(
    TArray<FVector> &line0,
    TArray<FVector> &line1
){
    /*
    array aufbau
    
    1   2-1  2

    0   3-0  3... paralelle lines, bilden quads durch adjazente indices
    
    heisst: 0 und 2 bzw 1 und 3 bilden bounding box
    */
    int limit = std::min(line0.Num(), line1.Num());
    for(int i = 1; i < limit; i++){
        FVector &v2 = line1[i];
        FVector &v0 = line0[i-1];

        if(true){
            /**
             * CAUTION: is still. Bugged.
             */

            FVector &v1 = line1[i-1];
            FVector &v3 = line0[i];
            TArray<FVector> positions = {v0, v1, v2, v3};
            TArray<chunk *> chunksCollected = terrainCreatorPtr->chunksAt(positions);

            for (int j = 0; j < chunksCollected.Num(); j++){
                chunk *currentChunk = chunksCollected[j];
                if(currentChunk != nullptr){
                    currentChunk->blockAreaForFoliage(v0, v1, v2, v3);
                }
            }
        
            
            /*else{
            //old
            //chunk 0
            chunk *ptr = chunkAt(
                cmToChunkIndex(v0.X),
                cmToChunkIndex(v0.Y)
            );
            if(ptr != nullptr){
                ptr->blockAreaForFoliage(v0,v2);
            }
            
            //chunk 1
            chunk *ptr1 = chunkAt(
                cmToChunkIndex(v2.X),
                cmToChunkIndex(v2.Y)
            );
            if(ptr1 != nullptr){
                ptr1->blockAreaForFoliage(v0,v2);
            }*/
        }
        
    }



}



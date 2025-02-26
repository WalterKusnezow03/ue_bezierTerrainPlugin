// Fill out your copyright notice in the Description page of Project Settings.


#include "RockCreator.h"

RockCreator::RockCreator()
{
}

RockCreator::~RockCreator()
{
}

MeshData RockCreator::createMesh(){
    int layers = 5;
    int detailStep = 100;
    int sizeX = 500;
    int sizeY = 500;
    int heightStep = 100;

    return createMesh(sizeX, sizeY, detailStep, layers, heightStep);
}



MeshData RockCreator::createMesh(int sizeX, int sizeY, int detailStep, int layers, int heightStep){
    if(layers == 0){
        return MeshData();
    }

    MeshData meshDataOut;

    std::vector<FVectorShape> shapes;
    //create shapes
    for (int i = 0; i < layers; i++){
        shapes.push_back(createShape(detailStep, sizeX, sizeY));
    }

    //find smallest count
    int smallestCount = shapes[0].vertexCount();
    for (int i = 1; i < shapes.size(); i++){
        int countNew = shapes[i].vertexCount();
        if (countNew < smallestCount)
        {
            smallestCount = countNew;
        }
    }

    //limit all to smallest count
    MMatrix up;
    for (int i = 0; i < shapes.size(); i++){
        FVectorShape &currentShape = shapes[i];
        currentShape.keepVertexCountFromFront(smallestCount);

        //move vertecies up
        up.setTranslation(0, 0, heightStep * i);
        currentShape.moveVerteciesWith(up);

        //append all data
        currentShape.joinMeshData(meshDataOut);
    }
    meshDataOut.calculateNormals();

    //close at bottom and top
    int layer0 = 0;
    int lastLayer = shapes.size() - 1;
    MeshData bottom = shapes[layer0].closeMeshAtCenter(false); //clockwise false, look to bottom
    MeshData top = shapes[lastLayer].closeMeshAtCenter(true); //clockwise true, look to top

    meshDataOut.append(bottom);
    meshDataOut.append(top);
    return meshDataOut;
}



/// @brief creates a smoothed shape in sizeX, sizeY and initial anchor detailStep
/// @param detailStep 
/// @param sizeX 
/// @param sizeY 
/// @return 
FVectorShape RockCreator::createShape(int detailStep, int sizeX, int sizeY){

    std::vector<FVector> sideStartPoints = MeshData::create2DQuadVertecies(sizeX, sizeY);


    FVectorShape outShape;
    for (int i = 0; i < sideStartPoints.size(); i++)
    {
        int next = (i + 1) % sideStartPoints.size(); //include edge from 3 to 0

        FVector start = sideStartPoints[i];
        FVector end = sideStartPoints[next];

        FVector direction = (end - start); // AB = B - A
        int countNow = direction.Size() / detailStep;
        direction /= countNow; //scale to correct offset per vertex

        addVertecies(
            outShape,
            direction,
            start,
            countNow
        );
    }
    outShape.randomizeVertecies(detailStep / 2);
    outShape.smoothWithBezier(detailStep / 10);

    //debug log vertex count
    FString message = FString::Printf(TEXT("debugShape vertex count %d"), outShape.vertexCount());
    DebugHelper::logMessage(message);

    return outShape;
}




void RockCreator::addVertecies(
    FVectorShape &shape,
    FVector &directionWithDistance,
    FVector &offset, 
    int count
){
    for (int i = 0; i < count; i++){
        FVector vertex = offset + i * directionWithDistance; //gx = A + r(B-A)
        shape.push_back(vertex);
    }
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "RockCreator.h"
#include "p2/meshgen/foliage/helper/GrahamScan.h"

RockCreator::RockCreator()
{
    rockMaxSize = 2000;
}

RockCreator::RockCreator(int maxSize)
{
    maxSize = std::abs(maxSize);
    if(maxSize > 100){
        rockMaxSize = maxSize;
    }
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

    //old
    
    //create shapes
    if(true){
        for (int i = 0; i < layers; i++){
            shapes.push_back(createShape(detailStep, sizeX, sizeY));
        }
    }
    //new
    if(false){
        createShapes(shapes, layers, detailStep);
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
    outShape.randomizeVertecies(detailStep / 2); //must be made more random
    outShape.smoothWithBezier(detailStep / 10); //must be made by some other function

    //debug log vertex count
    FString message = FString::Printf(TEXT("debugShape vertex count %d"), outShape.vertexCount());
    DebugHelper::logMessage(message);

    return outShape;
}



/// @brief adds an amount of vertecies in a scaled direction 
/// @param shape shape to add to
/// @param directionWithDistance direction to add to 
/// @param offset starting offset 
/// @param count count of vertecies 
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




/**
 * 
 * 
 * ---- NEW -----
 * 
 * 
 */

 void RockCreator::createShapes(std::vector<FVectorShape> &shapes, int layers, int detailStep){
    if(layers < 1){
        return;
    }
    FVectorShape baseShape = createRandomShape(detailStep);
    shapes.push_back(baseShape);
    // create shapes
    for (int i = 1; i < layers; i++){
        //MMatrix downScale;
        //downScale.scaleUniform(0.9f);
        //baseShape.moveVerteciesWith(downScale);

        FVectorShape copy = baseShape;
        copy.randomizeVertecies(detailStep);
        copy.smoothWithBezier(detailStep / 10); //must be made by some other function

        shapes.push_back(copy);
    }
 }




FVectorShape RockCreator::createRandomShape(int detailStep){
    //create random vertecies
    detailStep = std::abs(detailStep);
    int vertexCount = FVectorUtil::randomNumber(30, 50);
    std::vector<FVector> vertecies;
    for (int i = 0; i < vertexCount; i++){
        FVector newVertex = FVectorUtil::randomOffset(rockMaxSize);
        newVertex.Z = 0.0f; //remove z height
        vertecies.push_back(newVertex);
    }

    //convex hull sort
    GrahamScan scan;
    scan.ComputeConvexHull(vertecies);

    //filter for min distance
    FVectorShape shape;
    if (vertecies.size() > 0)
    {
        FVector &prev = vertecies[0];
        shape.push_back(prev);
        for (int i = 1; i < vertecies.size(); i++){
            FVector &current = vertecies[i];
            if (FVector::Dist(prev, current) >= detailStep){
                shape.push_back(current);
                prev = current;
            }
        }
    }

    shape.smoothWithBezier(detailStep / 10); // must be made by some other function
    shape.makeCenterPivot();

    return shape;
}

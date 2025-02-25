// Fill out your copyright notice in the Description page of Project Settings.


#include "RockCreator.h"

RockCreator::RockCreator()
{
}

RockCreator::~RockCreator()
{
}

MeshData RockCreator::createMesh(){
    MeshData outData;
    int layers = 5;
    for (int i = 0; i < layers; i++){
        bool closeShape = (i == layers - 1) || (i == 0);
        bool clockwiseClosed = (i == layers - 1); //oben versiegeln (ist ggf unten wenn i == 0)

        int detailStep = 100;
        int sizeX = 500;
        int sizeY = 500;

        createShapeAndAppend(
            outData, 
            i * 100, 
            closeShape, 
            clockwiseClosed,
            detailStep,
            sizeX,
            sizeY
        );
    }
    return outData;
}


/// @brief creates a new shape and appends it. Either open and or closed
/// @param other mesh to append to
/// @param zHeight height of the vertecies wanted
/// @param closeShape close this shape (at top or bottom)
/// @param clockwiseClosed only gets checked if closeShape is true (up normal or down normal vertex order)
void RockCreator::createShapeAndAppend(
    MeshData &other, 
    int zHeight, 
    bool closeShape, 
    bool clockwiseClosed,
    int detailStep,
    int sizeX,
    int sizeY
){
    
    FVectorShape shape = createShape(detailStep, sizeX, sizeY);

    MMatrix moveUp;
    moveUp.setTranslation(0, 0, zHeight);
    shape.moveVerteciesWith(moveUp);
    shape.joinMeshData(other); // merge to last layer


    if(closeShape){
        //close at top? true
        MeshData top = shape.closeMeshAtCenter(clockwiseClosed); 
        //achtung wenn unten dicht gemacht wird gibt es 
        //beim vertex append probleme
        //es muss anders geregelt werden ---> center at start?
        other.append(top);
    }
}

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


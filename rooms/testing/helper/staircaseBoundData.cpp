
#include "staircaseBoundData.h"
#include "p2/meshgen/MeshData.h"
#include "p2/DebugHelper.h"
#include "p2/util/TTouple.h"
#include "p2/entities/customIk/MMatrix.h"

template class TTouple<int, int>;

StaircaseBoundData::StaircaseBoundData(){

}

StaircaseBoundData::~StaircaseBoundData(){

}

void StaircaseBoundData::createLayout(int sizeXmeters, int sizeYmeters){
    //auto shape analyse machen-
    clearLayout(sizeXmeters, sizeYmeters);

    //richtung x hoch
    int i = 0;
    int j = 0;

    FVector2D dir(1, 0);
    while(i < sizeXmeters){

        //lower
        dir = FVector2D(1, 0);
        setSlope(i, 0, dir);

        //top
        dir = FVector2D(-1, 0);
        setSlope(i, sizeYmeters-1, dir);

        i++;
    }
    while(j < sizeYmeters){
        //lower
        dir = FVector2D(0, -1); //was flipped ? (0,1), (0,-1)
        setSlope(0, j, dir);

        //top
        dir = FVector2D(0, 1);
        setSlope(sizeXmeters-1, j, dir);

        j++;
    }



    //flat corners
    setCornersFlat();

    //printLayout();
}

void StaircaseBoundData::clearLayout(int sizeX, int sizeY){
    layout.clear();
    for (int i = 0; i < sizeX; i++){
        std::vector<Stair> inner(sizeY, Stair());
        layout.push_back(inner);
    }
}

bool StaircaseBoundData::indexIsValid(int i, int j){
    return i >= 0 && i < layout.size() && j >= 0 && j < layout[i].size();
}

void StaircaseBoundData::setCornersFlat(){
    int i0 = 0;
    int iMax = layout.size() - 1;
    if(iMax > 0){
        int j0 = 0;
        int jMax = layout[0].size() - 1;
        
        if(indexIsValid(i0, j0) && indexIsValid(iMax, jMax)){
            layout[i0][j0].isFlat = true;
            layout[iMax][j0].isFlat = true;
            layout[iMax][jMax].isFlat = true;
            layout[i0][jMax].isFlat = true;
        }
        
    }

}

void StaircaseBoundData::setFlat(int i, int j, FVector2D &dir){
    if(indexIsValid(i,j)){
        Stair stair(i,j,true);
        stair.direction = dir;
        layout[i][j] = stair;
    }
}

void StaircaseBoundData::setSlope(int i, int j, FVector2D &dir){
    if(indexIsValid(i,j)){
        Stair stair(i,j,false);
        stair.direction = dir;
        layout[i][j] = stair;
    }
} 

/**
 * helper for creating the vertecies
 */

///@brief creates staircase subpart and appends it into the meshdata,
///updates the passed height offset
void StaircaseBoundData::Stair::appendData(
    int oneMeter, 
    int slopeMeter, 
    MeshData &outdata,
    FVector &heightOffset
){
    if(!isNone){ //skip if none
        MMatrix heightOffsetMat(heightOffset);

        std::vector<FVector> quad = orientedAndMovedQuad(oneMeter);

        //move up.
        if(!isFlat){
            quad[1].Z += slopeMeter;
            quad[2].Z += slopeMeter;
            heightOffset.Z += slopeMeter;
        }

        for (int i = 0; i < quad.size(); i++){
            quad[i] = heightOffsetMat * quad[i]; //doesnt seem to work
        }

        outdata.appendDoublesided(
            quad[0],
            quad[1],
            quad[2],
            quad[3]
        );
    }
}

///@brief creates the quad in the given meter size and correct orientation
///aswell as the local transform in the array, scaled up to the given meter scale
std::vector<FVector> StaircaseBoundData::Stair::orientedAndMovedQuad(int oneMeter){
    std::vector<FVector> quad = {
        FVector(0, 0, 0),               // v0
        FVector(oneMeter, 0, 0),        // v1
        FVector(oneMeter, oneMeter, 0), // v2
        FVector(0, oneMeter, 0),        // v3
    };

    //to center, rotate around center of mass
    FVector toCenter(-oneMeter / 2.0f, -oneMeter / 2.0f, 0);
    MMatrix toCenterMat(toCenter);
    for (int i = 0; i < quad.size(); i++){
        quad[i] = toCenterMat * quad[i];
    }

    //important here: rotate around center of mass!
    MMatrix rot = rotation();
    for (int i = 0; i < quad.size(); i++){
        quad[i] = rot * quad[i];
    }

    FVector fromCenter = toCenter * -1.0f;
    MMatrix fromCenterMat(fromCenter);
    for (int i = 0; i < quad.size(); i++){
        quad[i] = fromCenterMat * quad[i];
    }


    //M = T * R <-- lese richtung --
    FVector pos(posX * oneMeter, posY * oneMeter, 0);
    MMatrix translateMat(pos);
    for (int i = 0; i < quad.size(); i++){
        quad[i] = translateMat * quad[i];
    }

    return quad;
}

MMatrix StaircaseBoundData::Stair::rotation(){
    FVector d3Vec(direction.X, direction.Y, 0);
    return MMatrix::createRotatorFrom(d3Vec);
}


MeshData StaircaseBoundData::generate(int oneMeter, int heightMetersUpperLimit, int maxSlopeMeters){

    //run trought the array and find the next connected part to determine the direction of the slope
    //or extend the flat area in

    //ACHTUNG: Berücksichtigt noch keine höhe! At all!
    MeshData outData;
    FVector maxHeightSave(0, 0, 0);

    //es muss korrekt an den kanten iteriert werden
    std::vector<TTouple<int, int>> indices;
    getIndicesClockwise(indices);
    for (int i = 0; i < indices.size(); i++){

        int icopy = indices[i].first();
        int jcopy = indices[i].last();
        if(indexIsValid(icopy, jcopy)){
            Stair &current = layout[icopy][jcopy];

            //make all flat after reached.
            if(maxHeightSave.Z >= heightMetersUpperLimit){
                current.isFlat = true;
            }

            //align to targeted height
            int nextSlope = maxHeightSave.Z + maxSlopeMeters;
            if (nextSlope > heightMetersUpperLimit)
            {
                maxSlopeMeters -= (nextSlope - heightMetersUpperLimit);
            }

            current.appendData(oneMeter, maxSlopeMeters, outData, maxHeightSave);
        }   
    }

    outData.calculateNormals();

    return outData;
}

///@brief gets the indicies where the staircase is placed in clockwise order for
///building the mesh
void StaircaseBoundData::getIndicesClockwise(
    std::vector<TTouple<int,int>> &output
){
    //erst x
    if(layout.size() > 0){
        int i = 0;
        int j = 0;

        //to x max
        while (i < layout.size() - 1){
            output.push_back(TTouple<int, int>(i, j));
            i++;
        }

        //to y max
        while (j < layout[0].size() - 1){
            output.push_back(TTouple<int, int>(i, j));
            j++;
        }

        //to x min
        while(i > 0){
            output.push_back(TTouple<int, int>(i, j));
            i--;
        }

        //to y min
        while(j > 0){
            output.push_back(TTouple<int, int>(i, j));
            j--;
        }

        //close
        output.push_back(output.front());
    }
}







//DEBUG
void StaircaseBoundData::printLayout(){

    //looks correct

    FString message = FString::Printf(TEXT("staircasedebug"));
    DebugHelper::logMessage(message);
    for (int i = 0; i < layout.size(); i++){
        FString messageSub;
        for (int j = 0; j < layout[i].size(); j++){
            Stair &current = layout[i][j];
            if (current.isNone == false){
                messageSub += "1";
            }else{
                messageSub += "0";
            }
        }
        messageSub += "|break staircasedebug|";
        DebugHelper::logMessage(messageSub);
    }
    
}
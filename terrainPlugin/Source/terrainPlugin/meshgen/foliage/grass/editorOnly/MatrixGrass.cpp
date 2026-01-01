#include "MatrixGrass.h"
#include "GameCore/util/FVectorUtil.h"

MatrixGrass::MatrixGrass(){

}

MatrixGrass::~MatrixGrass(){

}

MeshData MatrixGrass::Generate(int sizecm, int density, FVector2D pivotUv){
    pivotUv.X = std::clamp(pivotUv.X, 0.0, 1.0);
    pivotUv.Y = std::clamp(pivotUv.Y, 0.0, 1.0);


    MeshData outputData;

    //validate input data
    sizecm = std::abs(sizecm);
    density = std::abs(density);
    sizecm = std::max(sizecm, 1);
    density = std::max(density, 1);

    float fraction = sizecm / density;
    FVector normalizedDirectionX(fraction, 0.0f, 0.0f);
    FVector normalizedDirectionY(0.0f, fraction, 0.0f);
   

    FVector localCenter(sizecm, sizecm, 0);
    localCenter.X *= pivotUv.X;
    localCenter.Y *= pivotUv.Y;

    MMatrix toCenter;
    MMatrix offset;
    toCenter.setTranslation(toCenter);
    for (float i = 0; i < sizecm; i += fraction){
        FVector currentStepX = normalizedDirectionX * i;
        for (float j = 0; j < sizecm; j += fraction){
            FVector currentStepY = normalizedDirectionY * j;
            FVector currentStep = currentStepX + currentStepY;
            offset.setTranslation(currentStep);

            MMatrix M = toCenter * offset; // Tcenter * T1 <-- lese richtung
            CreateSprite(outputData, M);
        }
    }
    return outputData;
}











void MatrixGrass::CreateSingleStraw(MeshData &dataAppend, MMatrix &transform){

    //simple triangle for now / crossed planes
    MeshData tmpData;
    FVector v0(-3, 0, 0);
    FVector v1(FVectorUtil::randomFloatNumber(-1, 1), 0, FVectorUtil::randomFloatNumber(20, 40));
    FVector v2(3, 0, 0);
    tmpData.appendDoublesided(v0,v1,v2);

    MeshData tmpData1 = tmpData;
    MMatrix rotPi2;
    rotPi2.yawRadAdd(MMatrix::degToRadian(90.0f));
    tmpData1.transformAllVertecies(rotPi2);
    tmpData.append(tmpData1);

    tmpData.transformAllVertecies(transform);
    dataAppend.append(tmpData);
}


void MatrixGrass::CreateSprite(MeshData &dataAppend, MMatrix &transform){

    /*
    1-->2
    |   |
    0<--3
    */

    //simple triangle for now / crossed planes
    float up = FVectorUtil::randomFloatNumber(20, 40);
    MeshData tmpData;
    FVector v0(-20, 0, 0);
    FVector v1(-20, 0, up);
    FVector v2(20, 0, up);
    FVector v3(20, 0, 0);
    tmpData.appendDoublesided(v0,v1,v2,v3);

    FVector2D uv0(0.0f, 0.0f);
    FVector2D uv1(0.0f, 1.0f);
    FVector2D uv2(1.0f, 1.0f);
    FVector2D uv3(1.0f, 0.0f);
    tmpData.appendUvsDoubleSided(uv0, uv1, uv2, uv3);

    MeshData tmpData1 = tmpData;
    MMatrix rotPi2;
    rotPi2.yawRadAdd(MMatrix::degToRadian(90.0f));
    tmpData1.transformAllVertecies(rotPi2);
    tmpData.append(tmpData1);

    tmpData.transformAllVertecies(transform);
    dataAppend.append(tmpData);
}


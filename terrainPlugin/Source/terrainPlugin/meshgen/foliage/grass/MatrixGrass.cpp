#include "MatrixGrass.h"

MatrixGrass::MatrixGrass(){

}

MatrixGrass::~MatrixGrass(){

}

MeshData MatrixGrass::Generate(int sizecm, int density){
    MeshData outputData;

    //validate input data
    sizecm = std::abs(sizecm);
    density = std::abs(density);
    sizecm = std::max(sizecm, 1);
    density = std::max(density, 1);

    float fraction = sizecm / density;
    FVector normalizedDirectionX(fraction, 0.0f, 0.0f);
    FVector normalizedDirectionY(0.0f, fraction, 0.0f);
    float center = sizecm * 0.5f;

    FVector localCenter(center, center, 0);
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
            CreateSingleStraw(outputData, M);
        }
    }
    return outputData;
}

void MatrixGrass::CreateSingleStraw(MeshData &dataAppend, MMatrix &transform){

    //simple triangle for now / crossed planes
    MeshData tmpData;
    FVector v0(-3, 0, 0);
    FVector v1(0, 0, 10);
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

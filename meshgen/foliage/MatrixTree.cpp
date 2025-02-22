// Fill out your copyright notice in the Description page of Project Settings.


#include "MatrixTree.h"

MatrixTree::MatrixTree()
{
    stemCount = 5; //at least for dbeug
}

MatrixTree::~MatrixTree()
{
}

MeshData &MatrixTree::meshDataByReference(){
    return ownMeshData;
}

void MatrixTree::generate(int height, int cmPerStep){
    /**
     * hier matrizen in einer zahl generieren 
     * 
     */
    //sagen wir nach der hälfte ist der stamm zu ende
    //man braucht eine zahl an ästen

    int count = 40;
    for (int i = 0; i < count; i++)
    {
        MMatrix mat;
        mat.setTranslation(0, 0, cmPerStep);
        matrices.push_back(mat);
    }

    //create stem index
    stemCount = 5;
    IndexChain stem;
    for (int i = 0; i < stemCount; i++){
        stem.addIndex(i);
    }
    indexChains.push_back(stem);

    MMatrix stemStart = stemTop();

    //create randomRotations
    int prevRoll = 0;
    int prevPitch = 0;
    int prevYaw = 0;
    int rotMax = 90;
    for (int i = stemCount; i < matrices.size(); i++)
    {
        float roll = MMatrix::degToRadian(FVectorUtil::randomNumber(prevRoll - rotMax, prevRoll + rotMax));
        float pitch = MMatrix::degToRadian(FVectorUtil::randomNumber(prevPitch - rotMax, prevPitch + rotMax));
        float yaw = MMatrix::degToRadian(FVectorUtil::randomNumber(prevYaw - rotMax, prevYaw + rotMax));
        MMatrix &current = matrices[i];
        current.rollRadAdd(roll);
        current.pitchRadAdd(pitch);
        current.yawRadAdd(yaw);

        prevRoll = roll;
        prevPitch = pitch;
        prevYaw = yaw;
    }

    int subTrees = 6;
    for (int i = 0; i < subTrees; i++){
        IndexChain subtree;
        int subPartsPerTree = 10;
        for (int j = 0; j < subPartsPerTree; j++){
            int random = FVectorUtil::randomNumber(1, matrices.size() - 1); //only stem can have index 0 as start!
            if(indexIsValid(random)){
                subtree.addIndex(random);
            }
        }
        subtree.setOffsetMatrix(stemStart);
        indexChains.push_back(subtree);
    }

    generateMesh();
}

bool MatrixTree::indexIsValid(int index){
    return index >= 0 && index < matrices.size();
}

/// @brief will find the matrix by index and return the identity matrix if none could be found (invalid index)
/// @param index index in array
/// @return matrix found or identity, which must not be modified
MMatrix &MatrixTree::matrixByIndex(int index){
    if(indexIsValid(index)){
        return matrices[index];
    }
    return identityMatrix;
}

std::vector<FVector> MatrixTree::shapeByEnum(){
    std::vector<FVector> output;

    output.push_back(FVector(0,0,0));
    output.push_back(FVector(0,100,0));
    output.push_back(FVector(100,100,0));
    output.push_back(FVector(100,0,0));

    return output; //for vertecies sorroundign the shape
}


void MatrixTree::generateMesh(){
    /*
    achtung denkfehler: meshes überlappen sich ggf, die ketten müssen sinnvoll getrennt werden!
    man könnte für jeden starting index eine matrix speichern
    */
    ownMeshData.clearMesh();
    for (int i = 0; i < indexChains.size(); i++){
        IndexChain &indexChain = indexChains[i];
        std::vector<MMatrix> buildedChain = buildChain(indexChain);
        wrapWithMesh(buildedChain, ownMeshData);
    }
}

std::vector<MMatrix> MatrixTree::buildChain(IndexChain &indexChain){
    std::vector<MMatrix> outputChain;
    MMatrix current = indexChain.offsetMatrixRef();
    outputChain.push_back(current);
    std::vector<int> &chainRef = indexChain.indexChain();

    // build chain
    for (int i = 0; i < chainRef.size(); i++)
    {
        int index = chainRef[i];
        current *= matrixByIndex(index);
        outputChain.push_back(current);
    }
    return outputChain;
}


MMatrix MatrixTree::stemTop(){
    MMatrix mat;
    for (int i = 0; i < stemCount; i++){
        if(indexIsValid(i)){
            mat *= matrixByIndex(i);
        }
    }
    return mat;
}

void MatrixTree::wrapWithMesh(std::vector<MMatrix> &matricesIn, MeshData &mesh){

    if(matricesIn.size() > 1){
        std::vector<FVector> prevCircle = shapeByEnum();
        MMatrix &first = matricesIn[0];
        moveVerteciesFromLocalToWorld(first, prevCircle);

        for (int i = 1; i < matricesIn.size(); i++)
        {
            std::vector<FVector> currentCirlce = shapeByEnum();
            MMatrix &currentMatrix = matricesIn[i];
            moveVerteciesFromLocalToWorld(currentMatrix, currentCirlce);
            
            //join together and add to mesh
            join(prevCircle, currentCirlce, mesh);

            prevCircle = currentCirlce; //copy for next
        }
    }
}

void MatrixTree::moveVerteciesFromLocalToWorld(MMatrix &mat, std::vector<FVector> &vector){
    for (int i = 0; i < vector.size(); i++){
        FVector &current = vector[i];
        vector[i] = mat * current;
    }
}



void MatrixTree::join(std::vector<FVector> &lower, std::vector<FVector> &upper, MeshData &mesh){
    if(lower.size() == upper.size()){
        for (int i = 0; i < lower.size(); i++){
            /*
            1 2
            0 3
            */

            int next = (i + 1) % lower.size();
            FVector &b = lower[i];
            FVector &a = upper[i];
            FVector &d = upper[next];
            FVector &c = lower[next];

            mesh.append(a, b, c, d);
        }
    }
}
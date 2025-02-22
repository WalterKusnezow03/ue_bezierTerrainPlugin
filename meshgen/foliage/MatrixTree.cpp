// Fill out your copyright notice in the Description page of Project Settings.


#include "MatrixTree.h"
#include "ETreeType.h"
#include "p2/meshgen/foliage/helper/FVectorShape.h"

MatrixTree::MatrixTree()
{
    stemCountTop = 5; //at least for dbeug
}

MatrixTree::~MatrixTree()
{
}

MeshData &MatrixTree::meshDataByReference(){
    return ownMeshData;
}

void MatrixTree::generate(int height, int cmPerStep, ETreeType type){
    treeType = type;
    
    
    //erstmal matrizen generieren
    int count = 40;
    for (int i = 0; i < count; i++)
    {
        MMatrix mat;
        mat.setTranslation(0, 0, cmPerStep);
        matrices.push_back(mat);
    }



    //create stem
    stemCountTop = 5; //parts of the stem
    MMatrix identity;
    IndexChain stem = createSubTree(identity, stemCountTop);
    indexChains.push_back(stem);

    //create randomRotations
    randomRotationForAllMatrices();

    MMatrix stemStart = stemTop();
    int partsPerSubtree = 10;
    int subTrees = subTreeCountByEnum(treeType);
    createSubTrees(stemStart, partsPerSubtree, subTrees);

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

std::vector<FVectorShape> MatrixTree::shapeByEnum(ETreeType type){
    std::vector<FVectorShape> output;

    int size = 100;
    if(type == ETreeType::Edefault){
        size = 50;
        FVectorShape shape;
        shape.push_back(FVector(0,0,0));
        shape.push_back(FVector(0,size,0));
        shape.push_back(FVector(size,size,0));
        shape.push_back(FVector(size,0,0));
        output.push_back(shape);
    }
    if(type == ETreeType::EPalmTree){
        size = 20;
        int sizeInner = 10;

        FVectorShape shapeOuter;
        shapeOuter.push_back(FVector(0,0,0));
        shapeOuter.push_back(FVector(0,size,0));
        shapeOuter.push_back(FVector(size,size,0));
        shapeOuter.push_back(FVector(size,0,0));
        output.push_back(shapeOuter);

        FVectorShape shapeInner;
        shapeInner.push_back(FVector(0,0,0));
        shapeInner.push_back(FVector(0,sizeInner,0));
        shapeInner.push_back(FVector(sizeInner,sizeInner,0));
        shapeInner.push_back(FVector(sizeInner,0,0));
        output.push_back(shapeInner);

    }


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


/// @brief calculates the stem top location based on matricies 0 to stemCountTop
/// @return offsetMatrix top of stem
MMatrix MatrixTree::stemTop(){
    MMatrix mat;
    for (int i = 0; i < stemCountTop; i++){
        if(indexIsValid(i)){
            mat *= matrixByIndex(i);
        }
    }
    return mat;
}

void MatrixTree::wrapWithMesh(std::vector<MMatrix> &matricesIn, MeshData &mesh){

    if(matricesIn.size() > 1){

        //einfach alle shapes moven und dann connecten
        std::vector<FVectorShape> allShapes;
        for (int i = 0; i < matricesIn.size(); i++){
            MMatrix &currentMatrix = matricesIn[i];
            std::vector<FVectorShape> current = shapeByEnum(treeType);
            for (int j = 0; j < current.size(); j++){
                FVectorShape &currentShape = current[j];
                currentShape.moveVerteciesWith(currentMatrix);
                allShapes.push_back(currentShape);
            }
        }

        FVectorShape &prevShape = allShapes[0];
        for (int i = 1; i < allShapes.size(); i++){
            FVectorShape &currentShape = allShapes[i];

            //join together
            MeshData joinedMesh = currentShape.join(prevShape);
            mesh.append(joinedMesh);

            prevShape = currentShape;
        }
    }
}







void MatrixTree::createSubTrees(MMatrix &offset, int partsPerTree, int count){
    for (int i = 0; i < count; i++){
        IndexChain subtree = createSubTree(offset, partsPerTree);
        indexChains.push_back(subtree);
    }
}

IndexChain MatrixTree::createSubTree(MMatrix &offset, int parts){
    IndexChain subtree;
    subtree.setOffsetMatrix(offset);
    for (int i = 0; i < parts; i++){
        int random = FVectorUtil::randomNumber(1, matrices.size() - 1); //only stem can have index 0 as start!
        if(indexIsValid(random)){
            subtree.addIndex(random);
        }
    }
    return subtree;
}



int MatrixTree::rotationRangeByEnum(ETreeType type){
    if(type == ETreeType::Edefault){
        return 40;
    }
    if(type == ETreeType::EPalmTree){
        return 15;
    }
    return 10;
}

int MatrixTree::subTreeCountByEnum(ETreeType type){
    if(type == ETreeType::Edefault){
        return FVectorUtil::randomNumber(3,5);
    }
    if(type == ETreeType::EPalmTree){
        return FVectorUtil::randomNumber(0,2);
    }
    return 0;
}

void MatrixTree::randomRotationForAllMatrices(){
    int prevPitch = 0;
    int prevYaw = 0;
    int rotMax = rotationRangeByEnum(treeType);
    for (int i = 0; i < matrices.size(); i++)
    {
        float pitch = MMatrix::degToRadian(FVectorUtil::randomNumber(prevPitch - rotMax, prevPitch + rotMax));
        float yaw = MMatrix::degToRadian(FVectorUtil::randomNumber(prevYaw - rotMax, prevYaw + rotMax));
        MMatrix &current = matrices[i];
        current.pitchRadAdd(pitch);
        current.yawRadAdd(yaw);

        prevPitch = pitch;
        prevYaw = yaw;
    }
}
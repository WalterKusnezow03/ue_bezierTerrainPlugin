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

void MatrixTree::clean(){
    leafMeshData.clearMesh();
    ownMeshData.clearMesh();

    matrices.clear();
    indexChains.clear();
    leafTops.clear();
}

MeshData &MatrixTree::meshDataStemByReference(){
    return ownMeshData;
}

MeshData &MatrixTree::meshDataLeafByReference(){
    return leafMeshData;
}

void MatrixTree::generate(int height, int cmPerStep, ETreeType type){
    treeType = type;
    clean();

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

    stemTop = stem.endMatrixRef();
    createSubTrees(stemTop);

    generateMesh();
    generateLeafs();
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

        MMatrix offset;
        offset.setTranslation(- size / 2.0f, - size / 2.0f, 0);
        shapeOuter.moveVerteciesWith(offset);
        output.push_back(shapeOuter);

        FVectorShape shapeInner;
        shapeInner.push_back(FVector(0,0,0));
        shapeInner.push_back(FVector(0,sizeInner,0));
        shapeInner.push_back(FVector(sizeInner,sizeInner,0));
        shapeInner.push_back(FVector(sizeInner,0,0));


        offset.setTranslation(- sizeInner / 2.0f, - sizeInner / 2.0f, 0);
        shapeInner.moveVerteciesWith(offset);
        output.push_back(shapeInner);
    }


    return output; //for vertecies sorroundign the shape
}



void MatrixTree::generateMesh(){
    for (int i = 0; i < indexChains.size(); i++){
        IndexChain &indexChain = indexChains[i];
        wrapWithMesh(indexChain.matrixChain(), ownMeshData);
    }
}


void MatrixTree::wrapWithMesh(std::vector<MMatrix> &matricesIn, MeshData &mesh){

    if(matricesIn.size() > 1){

        //einfach alle shapes moven und dann connecten
        std::vector<FVectorShape> allShapes;
        
        for (int i = 0; i < matricesIn.size(); i++)
        {
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

        //add to leaf locations
        leafTops.push_back(matricesIn.back());
        std::vector<MMatrix> leaftopLocations;
        prevShape.copyVertecies(leaftopLocations);
        for (int i = 0; i < leaftopLocations.size(); i++){
            leafTops.push_back(leaftopLocations[i]);
        }
    }
}



void MatrixTree::createSubTrees(MMatrix &offset){
    int partsPerSubtree = partsPerSubTreeByEnum(treeType);
    int subTrees = subTreeCountByEnum(treeType);
    createSubTrees(offset, partsPerSubtree, subTrees);
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
    buildChain(subtree);
    return subtree;
}

/// @brief builds the chain from the index list and updates the inner matrix chain aswell as the end index
/// @param indexChain 
void MatrixTree::buildChain(IndexChain &indexChain){
    indexChain.resetMatrices();

    MMatrix current = indexChain.offsetMatrixRef();
    indexChain.addMatrix(current);
    std::vector<int> &chainRef = indexChain.indexChain();

    // build chain
    for (int i = 0; i < chainRef.size(); i++)
    {
        int index = chainRef[i];
        current *= matrixByIndex(index);
        indexChain.addMatrix(current);
    }
    indexChain.updateEndMatrix(current);
}




int MatrixTree::rotationRangeByEnum(ETreeType type){
    if(type == ETreeType::Edefault){
        return 30;
    }
    if(type == ETreeType::EPalmTree){
        return 40;
    }
    return 10;
}

int MatrixTree::subTreeCountByEnum(ETreeType type){
    if(type == ETreeType::Edefault){
        return FVectorUtil::randomNumber(3,5);
    }
    if(type == ETreeType::EPalmTree){
        int count = FVectorUtil::randomNumber(0,2);
        if(count == 1){
            return 0;
        }
    }
    return 0;
}

int MatrixTree::partsPerSubTreeByEnum(ETreeType type){
    if(type == ETreeType::Edefault){
        return FVectorUtil::randomNumber(3,5);
    }
    if(type == ETreeType::EPalmTree){
        return FVectorUtil::randomNumber(8,10);
    }
    return 5;
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



MMatrix MatrixTree::randomRotator(){
    MMatrix rotator;
    rotator.rollRadAdd(MMatrix::degToRadian(FVectorUtil::randomNumber(-90,90)));
    rotator.pitchRadAdd(MMatrix::degToRadian(FVectorUtil::randomNumber(-90,90)));
    rotator.yawRadAdd(MMatrix::degToRadian(FVectorUtil::randomNumber(-90,90)));
    return rotator;
}

MMatrix MatrixTree::randomRotator(int lower, int heigher){
    MMatrix rotator;
    rotator.rollRadAdd(MMatrix::degToRadian(FVectorUtil::randomNumber(lower,heigher)));
    rotator.pitchRadAdd(MMatrix::degToRadian(FVectorUtil::randomNumber(lower,heigher)));
    rotator.yawRadAdd(MMatrix::degToRadian(FVectorUtil::randomNumber(lower,heigher)));
    return rotator;
}



/**
 * 
 * --- leaf generation section ---
 * 
 */
void MatrixTree::generateLeafs(){
    for (int i = 0; i < leafTops.size(); i++)
    {
        MMatrix &ref = leafTops[i];
        for (int j = 0; j < leafCountPerJoint; j++){
            generateLeaf(ref);
        }
    }
}

void MatrixTree::generateLeaf(MMatrix &offset){
    //M = T * R <-- lese richtung --
    MMatrix rotation = randomRotator(-90, 90);
    MMatrix offsetFinal = offset * rotation;

    FVectorShape shape = leafShapeByEnum(treeType);
    shape.moveVerteciesWith(offsetFinal);

    MeshData closedLeaf = shape.createDoubleSidedMesh();
    leafMeshData.append(closedLeaf);
}



/// @brief generates a leaf shape by enum type
/// @param type 
/// @return 
FVectorShape MatrixTree::leafShapeByEnum(ETreeType type){
    FVectorShape output;

    if(type == ETreeType::Edefault){

    }
    if(type == ETreeType::EPalmTree){
        FVectorShape oneSide;
        oneSide.push_back(FVector(0, 0, 0));
        oneSide.push_back(FVector(-10, 0, 20));
        oneSide.push_back(FVector(-10, 0, 40));
        oneSide.push_back(FVector(-5, 0, 60));
        oneSide.push_back(FVector(0, 0, 70));
        output = oneSide;
        
        //flip shape for copy
        MMatrix flip;
        flip.scale(-1.0f, 1.0f, 1.0f);
        oneSide.moveVerteciesWith(flip);

        //rotate vertecies to create folding leaf
        MMatrix rotateLeft;
        rotateLeft.yawRadAdd(MMatrix::degToRadian(40));
        MMatrix rotateRight;
        rotateRight.yawRadAdd(MMatrix::degToRadian(-40));
        output.moveVerteciesWith(rotateLeft);
        oneSide.moveVerteciesWith(rotateRight);

        //merge both leafs
        output.push_back(oneSide); 


        MMatrix scaleUp;
        scaleUp.scaleUniform(4.0f);
        output.moveVerteciesWith(scaleUp);
    }

    return output;
}

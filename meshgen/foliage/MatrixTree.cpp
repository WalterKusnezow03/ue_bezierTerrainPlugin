// Fill out your copyright notice in the Description page of Project Settings.


#include "MatrixTree.h"
#include "ETreeType.h"
#include "p2/meshgen/foliage/ETerrainType.h"
#include "p2/meshgen/foliage/helper/IndexChain.h"
#include "p2/meshgen/foliage/helper/TreeProperties.h"
#include "p2/meshgen/foliage/helper/FVectorShape.h"

MatrixTree::MatrixTree()
{
    loadProperties();
    //erstmal matrizen generieren
    int count = 40;
    for (int i = 0; i < count; i++)
    {
        MMatrix mat;
        matrices.push_back(mat);
    }
}

MatrixTree::~MatrixTree()
{
}

/// @brief creates presets for different tree types
void MatrixTree::loadProperties(){
    /*
    constructor:
    TreeProperties(
		int detailstep, 
		ETreeType typeIn, 
		ETerrainType terrainTypeIn,
		int leafcountPerJointIn,
		int partsPerSubtreeIn,
		int subTreeCountIn
	);
    
    */
    TreeProperties palmProperty(100, ETreeType::EPalmTree, ETerrainType::ETropical, 8, 8, 3);
    palmProperty.setTargetedMaterials(materialEnum::treeMaterial, materialEnum::palmLeafMaterial);
    palmProperty.addTerrainType(ETerrainType::EDesert); // additional types for more than one terrain
    addPropertyToMap(palmProperty);

    TreeProperties oakProperty(100, ETreeType::Edefault, ETerrainType::ETropical, 30, 10, 3);
    oakProperty.addTerrainType(ETerrainType::EForest);
    oakProperty.setTargetedMaterials(materialEnum::treeMaterial, materialEnum::palmLeafMaterial);
    defaultProperty = oakProperty;
    addPropertyToMap(oakProperty);

    TreeProperties palmBush(100, ETreeType::EPalmBush, ETerrainType::ETropical, 8, 2, 1);
    palmBush.setTargetedMaterials(materialEnum::treeMaterial, materialEnum::palmLeafMaterial);
    addPropertyToMap(palmBush);


    TreeProperties cactus(100, ETreeType::ECactus, ETerrainType::EDesert, 0, 3, 3);
    cactus.setTargetedMaterials(materialEnum::palmLeafMaterial, materialEnum::palmLeafMaterial);
    addPropertyToMap(cactus);
}

void MatrixTree::addPropertyToMap(TreeProperties &property){

    std::vector<ETerrainType> &typesOfTerrain = property.getTerrainTypes();
    for (int i = 0; i < typesOfTerrain.size(); i++){
        ETerrainType typeOfTerrain = typesOfTerrain[i];

        if(terrainPropertyMap.find(typeOfTerrain) == terrainPropertyMap.end()){
            std::vector<TreeProperties> newVec;
            newVec.push_back(property);
            terrainPropertyMap[typeOfTerrain] = newVec;
        }else{
            std::vector<TreeProperties> &vec = terrainPropertyMap[typeOfTerrain];
            vec.push_back(property);
        }
    }
    
}

/// @brief founds a random tree / preset from the map by terrain type
/// @param typeOfTerrain type of terrain wanted
/// @return Preset found or default property
TreeProperties &MatrixTree::findProperty(ETerrainType typeOfTerrain){
    //default
    if(terrainPropertyMap.find(typeOfTerrain) == terrainPropertyMap.end()){
        return defaultProperty;
    }

    std::vector<TreeProperties> &vec = terrainPropertyMap[typeOfTerrain];
    if(vec.size() > 0){
        int index = FVectorUtil::randomNumber(0, vec.size());
        if(index >= 0 && index < vec.size()){
            return vec[index];
        }
    }
    return defaultProperty;
}



/// @brief cleans all leaf and mesh data
void MatrixTree::clean(){
    leafMeshData.clearMesh();
    stemMeshData.clearMesh();

    //matrices.clear();
    indexChains.clear();
    leafTops.clear();
}

/// @brief returns the mesh data for the "wood"
/// @return mesh data by reference
MeshData &MatrixTree::meshDataStemByReference(){
    return stemMeshData;
}

/// @brief returns the mesh data for the "leaf"
/// @return mesh data by reference
MeshData &MatrixTree::meshDataLeafByReference(){
    return leafMeshData;
}


/// @brief generates a tree by the terrain type
/// @param height 
/// @param cmPerStep 
/// @param terrainType 
void MatrixTree::generate(ETerrainType terrainType){
    clean();
    TreeProperties &properties = findProperty(terrainType);
    processAndGenerate(properties);
}



void MatrixTree::processAndGenerate(TreeProperties &properties){
    clean();
    stemMeshData.setTargetMaterial(properties.targetMaterialForStem());
    leafMeshData.setTargetMaterial(properties.targetMaterialForLeaf());


    int cmPerStep = properties.getDetailStep();
    treeType = properties.getTreeType();

    //create randomRotations
    randomRotationForAllMatrices();

    //erstmal matrizen verschieben
    for (int i = 0; i < matrices.size(); i++)
    {
        MMatrix &mat = matrices[i];
        mat.setTranslation(0, 0, cmPerStep);
    }

    //create stem
    MMatrix identity;
    IndexChain stem = createSubTree(identity, properties);
    indexChains.push_back(stem);
    stemTop = stem.endMatrixRef();

    createSubTrees(stemTop, properties);

    //generate stem mesh
    generateMesh();

    if(properties.getTreeType() == ETreeType::ECactus){
        generateCactusSpikes();
    }else{
        generateLeafs(properties);
    }
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




/// @brief generates the stem meshes from the created index chains
void MatrixTree::generateMesh(){
    for (int i = 0; i < indexChains.size(); i++){
        IndexChain &indexChain = indexChains[i];
        wrapWithMesh(indexChain.matrixChain(), stemMeshData);
    }
}

/// @brief pass in the vector of INTERPOLATED / concatenated matricies (multiplied), translation will be copied
/// @param matricesIn matricies which are only translation copies of multiplied once
/// @param mesh mesh data to append the stem to
void MatrixTree::wrapWithMesh(std::vector<MMatrix> &matricesIn, MeshData &mesh){

    MeshData subMesh;
    if (matricesIn.size() > 1)
    {

        //einfach alle shapes moven und dann connecten nacheinander
        std::vector<FVectorShape> allShapes;
        
        for (int i = 0; i < matricesIn.size(); i++)
        {
            MMatrix &currentMatrix = matricesIn[i];
            std::vector<FVectorShape> current = StemShapeByEnum(treeType);
            for (int j = 0; j < current.size(); j++){
                FVectorShape &currentShape = current[j];
                currentShape.moveVerteciesWith(currentMatrix);
                allShapes.push_back(currentShape);
            }
        }

        if(allShapes.size() > 0){
            
            for (int i = 0; i < allShapes.size(); i++)
            { 
                FVectorShape &currentShape = allShapes[i];
                currentShape.joinMeshData(subMesh);

                //close top shape
                if(i == allShapes.size() - 1){
                    MeshData closedTop = currentShape.closeMeshAtCenter(true);
                    subMesh.append(closedTop);
                }
            }

            //add to leaf locations
            leafTops.push_back(matricesIn.back());
            
            FVectorShape &lastShape = allShapes.back();
            std::vector<MMatrix> leaftopLocations;
            lastShape.copyVertecies(leaftopLocations);

            for (int i = 0; i < leaftopLocations.size(); i++){
                leafTops.push_back(leaftopLocations[i]);
            }
        }
        
        subMesh.calculateNormals();
    }

    //mesh.append(subMesh); //add new subtree mesh to data
    mesh.appendEfficent(subMesh); //NEW
}

/// @brief creates subtrees for a desired tree property
/// @param offset offset to have the subtree for
/// @param prop properties
void MatrixTree::createSubTrees(MMatrix &offset, TreeProperties &prop){
    for (int i = 0; i < prop.subTreeCount(); i++){
        IndexChain subtree = createSubTree(offset, prop);
        indexChains.push_back(subtree);
    }
}

/// @brief creates a subtree (index chain) with the index and matricies build up
/// @param offset offset to start the sub tree at
/// @param prop properties of the tree
/// @return new sub tree
IndexChain MatrixTree::createSubTree(MMatrix &offset, TreeProperties &prop){
    IndexChain subtree;
    subtree.setOffsetMatrix(offset);

    for (int i = 0; i < prop.partsPerSubtree(); i++){
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
        return 7;
    }
    if(type == ETreeType::EPalmTree){
        return 15;
    }
    return 10;
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
void MatrixTree::generateLeafs(TreeProperties &properties){
    for (int i = 0; i < leafTops.size(); i++)
    {
        MMatrix &ref = leafTops[i];
        for (int j = 0; j < properties.leafCountPerJoint(); j++){
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


/**
 * 
 * --- other sepcial types: cactus ---
 * 
 * 
 */

/// @brief generates the cactus mesh inside the leaf mesh, requires the
/// stem mesh to be builded
void MatrixTree::generateCactusSpikes(){
    /**
     * 
     * irgendwas funktioniert nicht...
     * 
     */

    //only one spike per matrix now
    //base shape:

    int width = 4;
    int height = 20;
    FVectorShape lower;
    lower.createQuadShape(width);

    FVectorShape upper = lower;
    MMatrix upMat;
    upMat.setTranslation(0, 0, height);
    MMatrix scaleMat;
    scaleMat.scale(0.1f, 0.1f, -1.0f);
    MMatrix combinedMat = upMat * scaleMat; //M = T * S <-- lese richtung --
    upper.moveVerteciesWith(combinedMat);

    MeshData sampleSpike;
    lower.joinMeshData(sampleSpike);
    upper.joinMeshData(sampleSpike); //same vertex count, join works fine here.

    MeshData topClose = upper.closeMeshAtCenter(true); //clockwise closing top (face upwards)
    sampleSpike.appendEfficent(topClose);
    sampleSpike.calculateNormals();

    // orient spike along axis for the placement with extracted rotations from normal
    // to work correctly!
    MMatrix rotToXAxis;
    rotToXAxis.pitchRadAdd(MMatrix::degToRadian(-90)); //clock wise
    sampleSpike.transformAllVertecies(rotToXAxis);


    
    // copy and move spike to position as wanted
    std::vector<MMatrix> transformMatrices;
    //stemMeshData.generateMatricesPerFaceAndLookDirOfNormal(transformMatrices);

    int stepSizeBetweenVertecies = height; //is random...
    stemMeshData.generateMatricesPerFaceAndLookDirOfNormal(
        transformMatrices
        //,stepSizeBetweenVertecies
    );

    for (int i = 0; i < transformMatrices.size(); i++)
    {
        MMatrix &currentMatrix = transformMatrices[i];
        MeshData copy = sampleSpike;
        copy.transformAllVertecies(currentMatrix);
        
        leafMeshData.append(copy);
    }

    //debug
    //FString message = FString::Printf(TEXT("size of matrices: %d"), transformMatrices.size()); //500
    //DebugHelper::logMessage(message);
}

/**
 * 
 * 
 * ----- SHAPE PRESETS -----
 * 
 * 
 */

/// @brief creates a stem shape with the pivot at 0,0,0 and mesh reaching upwards
/// @param type 
/// @return 
std::vector<FVectorShape> MatrixTree::StemShapeByEnum(ETreeType type){
    std::vector<FVectorShape> output;

    int size = 100;
    if(type == ETreeType::Edefault){
        size = 50;
        FVectorShape shape;
        shape.push_back(FVector(0, 0, 0));
        shape.push_back(FVector(0,size,0));
        shape.push_back(FVector(size,size,0));
        shape.push_back(FVector(size,0,0));
        

        output.push_back(shape);
        
    }
    if(type == ETreeType::EPalmTree || type == ETreeType::EPalmBush){
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
    if(type == ETreeType::ECactus){
        size = 20;
        int innerSize = 15;
        int detail = 12;
        FVectorShape shapeCircle;
        //shapeCircle.createCircleShape(size, detail);
        shapeCircle.createSpikedCircleShape(size, innerSize, detail);

        output.push_back(shapeCircle);
    }

    return output; //for vertecies sorroundign the shape
}



/// @brief generates a leaf shape by enum type, with the pivot at 0,0,0 and mesh reaching upwards
/// @param type 
/// @return 
FVectorShape MatrixTree::leafShapeByEnum(ETreeType type){
    FVectorShape output;

    if(type == ETreeType::Edefault){
        FVectorShape oneSide;
        oneSide.push_back(FVector(0, 0, 0));
        oneSide.push_back(FVector(-10, 0, 10));
        oneSide.push_back(FVector(-10, 0, 20));
        oneSide.push_back(FVector(-5, 0, 30));
        oneSide.push_back(FVector(0, 0, 35));
        output = oneSide; //copy
        
        //flip shape for copy
        MMatrix flip;
        flip.scale(-1.0f, 1.0f, 1.0f);
        oneSide.moveVerteciesWith(flip);

        output.push_back(oneSide); 
        MMatrix scaleUp;

        int scaleUpRand = FVectorUtil::randomNumber(3, 10);
        scaleUp.scaleUniform(scaleUpRand);
        output.moveVerteciesWith(scaleUp);



    }
    if(type == ETreeType::EPalmTree || type == ETreeType::EPalmBush){
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

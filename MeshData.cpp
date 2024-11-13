// Fill out your copyright notice in the Description page of Project Settings.

#include "p2/meshgen/meshDataContainer/Quad.h"
#include "p2/meshgen/MeshData.h"

MeshData::MeshData()
{
}

MeshData::MeshData(TArray<FVector> &&verteciesIn, TArray<int> &&trianglesIn){
    setVertecies(MoveTemp(verteciesIn));
    setTriangles(MoveTemp(trianglesIn));
}

MeshData::~MeshData()
{
    clearMesh();
}


/// @brief copy constructor gets called if passing by value
/// @param other 
MeshData::MeshData(const MeshData &other){
    *this = other;
}

/// @brief abusing the assign operator to create a deepcopy of the data
/// @param other 
/// @return 
MeshData& MeshData::operator=(const MeshData &other){
    if(&other != this){
        clearMesh(); //clear mesh data before adding any data!

        for (int i = 0; i < other.vertecies.Num(); i++){
            vertecies.Add(other.vertecies[i]);
        }
        for (int i = 0; i < other.triangles.Num(); i++){
            triangles.Add(other.triangles[i]);
        }
        for (int i = 0; i < other.normals.Num(); i++){
            normals.Add(other.normals[i]);
        }
        for (int i = 0; i < other.UV0.Num(); i++){
            UV0.Add(other.UV0[i]);
        }
        for (int i = 0; i < other.Tangents.Num(); i++){
            Tangents.Add(other.Tangents[i]);
        }
        for (int i = 0; i < other.VertexColors.Num(); i++){
            VertexColors.Add(other.VertexColors[i]);
        }
    }
    return *this;
}


/// @brief clean up data completely
void MeshData::clearMesh(){
    vertecies.Empty();
    triangles.Empty();
    normals.Empty();

    UV0.Empty();
    Tangents.Empty();
    VertexColors.Empty();
}

/// @brief clears the normals 
void MeshData::clearNormals(){
    normals.Empty();
}

/**
 * --- apply data ---
 */

/// @brief calculates the normals and applies it to the vertecies
void MeshData::calculateNormals(){
    UKismetProceduralMeshLibrary::CalculateTangentsForMesh(vertecies, triangles, UV0, normals, Tangents);
}

/// @brief sets the data for all vertecies, pass by r value reference
/// @param vertecies veretecies to set, be carefull with overriding
void MeshData::setVertecies(TArray<FVector> &&verteciesIn){
    vertecies = MoveTemp(verteciesIn);  // Move the data instead of copying, creating an r value
}
/// @brief sets the data for all triangles, pass by r value reference
/// @param trianglesIn triangles to set for the mesh
void MeshData::setTriangles(TArray<int32> &&trianglesIn){
    triangles = MoveTemp(trianglesIn);
}



///join another mesh, vertecies add, triangles added with offset added to index
void MeshData::append(MeshData &other){
    TArray<FVector> &verteciesRef = other.getVerteciesRef();
    TArray<int32> &trianglesRef = other.getTrianglesRef();
    join(verteciesRef, trianglesRef);
}

/// join another quad to mesh, vertecies add, triangles added with offset added to index
void MeshData::append(Quad &&other){
    quads.push_back(MoveTemp(other));

    Quad &otherRef = quads[quads.size() - 1];

    TArray<FVector> &verteciesRef = otherRef.readVertecies();
    TArray<int32> &trianglesRef = otherRef.readTriangles();
    join(verteciesRef, trianglesRef);
}



void MeshData::join(TArray<FVector> &verteciesRef, TArray<int32> &trianglesRef){
    int triangleOffset = triangles.Num();

    //copy triangles, apply offset
    for (int i = 0; i < trianglesRef.Num(); i++){
        int32 copy = trianglesRef[i];
        copy += triangleOffset;
        triangles.Add(copy);
    }

    //copy vertecies
    for(int i = 0; i < verteciesRef.Num(); i++){
        FVector &ref = verteciesRef[i];
        vertecies.Add(ref);
    }
}








/**
 * --- Data references ---
 */

/// @brief returns the reference to the mesh data vertecies, be carefull with modifying
/// @return mesh data vertecies by reference
TArray<FVector> &MeshData::getVerteciesRef(){
    return vertecies;
}

/// @brief returns the reference to the mesh data triangles, be carefull with modifying
/// @return mesh data triangles by reference
TArray<int32> &MeshData::getTrianglesRef(){
    return triangles;
}

TArray<FVector> &MeshData::getNormalsRef(){
    return normals;
}

TArray<FVector2D> &MeshData::getUV0Ref(){
    return UV0;
}
TArray<FProcMeshTangent> &MeshData::getTangentsRef(){
    return Tangents;
}
TArray<FColor> &MeshData::getVertexColorsRef(){
    return VertexColors;
}





void MeshData::rebuildMeshDataFromQuads(){
    clearMesh();
    for (int i = 0; i < quads.size(); i++){
        Quad &currentQuad = quads[i];
        TArray<FVector> &verteciesRef = currentQuad.readVertecies();
        TArray<int32> &trianglesRef = currentQuad.readTriangles();
        join(verteciesRef, trianglesRef);
    }
}


/// @brief will process the hit if needed, and return if the mesh needs to be reloaded
/// @param localHitpoint 
/// @param direction 
/// @return 
bool MeshData::processHit(FVector &localHitpoint, FVector &direction){
    std::vector<int> closestQuadsIndices;
    findClosestQuadsTo(localHitpoint, closestQuadsIndices);

    if(closestQuadsIndices.size() > 0){

        for (int i = 0; i < closestQuadsIndices.size(); i++){

            int index = closestQuadsIndices[i];
            if(index >= 0 && index < quads.size()){
                Quad &currentQuad = quads[index];
                bool withinQuad = currentQuad.isWithinQuad(localHitpoint);
                if(withinQuad){
                    //todo:
                    //erase from quads

                    //create new split up mesh

                    //return true
                    return true;
                }
            }

        }
    }
    return false;
}

/// @brief finds closest Quads to a local hitpoint, really important that it is a local one!
/// WARNING: DO NOT USE THE POINTERS OTHER FOR TEMPORAL USAGE! MIGHT GET INVALID!
/// @param localHitpoint 
void MeshData::findClosestQuadsTo(
    FVector &localHitpoint,
    std::vector<int> &outputindices 
){
    if(quads.size() <= 0){
        return;
    }

    
    Quad *firstQuad = &quads[0];
    float prevDistance = FVector::Dist(localHitpoint, firstQuad->center());
    outputindices.push_back(0);


    for (int i = 1; i < quads.size(); i++)
    {
        Quad &currentQuad = quads[i];
        float distance = FVector::Dist(currentQuad.center(), localHitpoint);
        if(distance < prevDistance){
            distance = prevDistance;
            outputindices.push_back(i);
        }
    }

}




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
    // Stelle sicher, dass die Normalen-Array-Größe stimmt
    normals.SetNum(vertecies.Num());

    // Iteriere über die Dreiecke und berechne Normalen
    if(vertecies.Num() == triangles.Num()){
        for (int i = 0; i < triangles.Num() - 2; i += 3) {
            int32 Index0 = triangles[i];
            int32 Index1 = triangles[i + 1];
            int32 Index2 = triangles[i + 2];

            FVector Edge1 = vertecies[Index1] - vertecies[Index0];
            FVector Edge2 = vertecies[Index2] - vertecies[Index0];

            FVector Normal = FVector::CrossProduct(Edge1, Edge2).GetSafeNormal();

            //summieren der alten normale, normalisieren
            /*normals[Index0] = (normals[Index0] + Normal).GetSafeNormal();
            normals[Index1] = (normals[Index1] + Normal).GetSafeNormal();
            normals[Index2] = (normals[Index2] + Normal).GetSafeNormal();*/
            normals[Index0] = Normal;
            normals[Index1] = Normal;
            normals[Index2] = Normal;
        }
    }
    




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





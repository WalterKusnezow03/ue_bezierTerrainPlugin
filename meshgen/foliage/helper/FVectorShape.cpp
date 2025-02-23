// Fill out your copyright notice in the Description page of Project Settings.


#include "FVectorShape.h"
#include "p2/meshgen/MeshData.h"

FVectorShape::FVectorShape()
{
}

FVectorShape::~FVectorShape()
{
}

FVectorShape::FVectorShape(const FVectorShape &other){
    if(&other != this){
        *this = other;
    }
}

FVectorShape::FVectorShape(const FVectorShape &other, MMatrix &applyOffset){
    if(&other != this){
        *this = other;
    }
    moveVerteciesWith(applyOffset);
}

FVectorShape &FVectorShape::operator=(const FVectorShape &other){
    if(&other == this){
        return *this;
    }
    vec = other.vec;
    return *this;
}

/// @brief moves the vertecies with a desired matrix
/// @param mat 
void FVectorShape::moveVerteciesWith(MMatrix &mat){
    for (int i = 0; i < vec.size(); i++){
        FVector &current = vec[i];
        vec[i] = mat * current;
    }
}



void FVectorShape::push_back(FVector other){
    vec.push_back(other);
}

void FVectorShape::push_back(FVectorShape &other){
    for (int i = 0; i < other.vec.size(); i++){
        vec.push_back(other.vec[i]);
    }
}

/// @brief will joint this and the other shape together AND create the needed normals!
/// @param other other shape to connect to
/// @return new meshdata
MeshData FVectorShape::join(FVectorShape &other){
    MeshData mesh;
    if(other.vec.size() > 0){
        for (int i = 0; i < vec.size(); i++)
        {
            /*
            1 2    i, next
            0 3    i, next
            */
            int iOther = i % other.vec.size();
            int nextThis = (i + 1) % vec.size();
            int nextOther = (i + 1) % other.vec.size();

            FVector &a = vec[i];
            FVector &b = other.vec[iOther];
            FVector &c = other.vec[nextOther];
            FVector &d = vec[nextThis];

            mesh.append(a, b, c, d);
        }
    }
    mesh.calculateNormals();
    return mesh;
}



// new method to join smoothley
void FVectorShape::joinMeshData(MeshData &other){
    other.appendVertecies(vec);
}

/// @brief creates a double sided mesh from own shape and center of mass
/// @return shape Mesh data, double sided
MeshData FVectorShape::createDoubleSidedMesh(){
    MeshData mesh;
    
    //center of mass
    FVector center;
    for (int i = 0; i < vec.size(); i++){
        center += vec[i];
    }
    center /= vec.size();

    // connect triangles to center
    for (int i = 1; i < vec.size(); i++){
        //create double sided triangle
        FVector &prev = vec[i - 1];
        FVector &current = vec[i];
        mesh.appendDoublesided(center, prev, current);
    }

    mesh.calculateNormals();
    return mesh;
}




void FVectorShape::copyVertecies(std::vector<MMatrix> &output){
    for (int i = 0; i < vec.size(); i++){
        MMatrix mat;
        mat.setTranslation(vec[i]);
        output.push_back(mat);
    }
}
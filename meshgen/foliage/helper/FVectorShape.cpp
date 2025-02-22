// Fill out your copyright notice in the Description page of Project Settings.


#include "FVectorShape.h"
#include "p2/meshgen/MeshData.h"

FVectorShape::FVectorShape()
{
}

FVectorShape::~FVectorShape()
{
}


void FVectorShape::push_back(FVector other){
    vec.push_back(other);
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

/// @brief moves the vertecies with a desired matrix
/// @param mat 
void FVectorShape::moveVerteciesWith(MMatrix &mat){
    for (int i = 0; i < vec.size(); i++){
        FVector &current = vec[i];
        vec[i] = mat * current;
    }
}
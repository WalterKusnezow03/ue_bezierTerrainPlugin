// Fill out your copyright notice in the Description page of Project Settings.



#include "p2/meshgen/meshDataContainer/Quad.h"
#include "CoreMinimal.h"

Quad::Quad(
    FVector &a,
    FVector &b,
    FVector &c,
    FVector &d
){
    createQuad(a,b,c,d);
}

Quad::~Quad()
{
}

//refs
TArray<FVector> &Quad::readVertecies(){
    return vertecies;
}

TArray<int32> &Quad::readTriangles(){
    return triangles;
}


/// @brief all quads MUST BE BUILD out of TRIANGLES, OTHERWISE MANY BUGS OCCUR!
/// @param a corner 0
/// @param b corner 1
/// @param c corner 2
/// @param output output to append in
/// @param trianglesOutput triangle int32 as nums saved in here, also appended
void Quad::buildTriangle(
    FVector &a, 
    FVector &b, 
    FVector &c,
    TArray<FVector> &output,
    TArray<int32> &trianglesOutput
){
    //add vertecies
    output.Add(a);
    output.Add(b);
    output.Add(c);

    //add triangles
    int32 offset = trianglesOutput.Num();

    trianglesOutput.Add(0 + offset); // 0th vertex in the first triangle
    trianglesOutput.Add(1 + offset); // 1st vertex in the first triangle
    trianglesOutput.Add(2 + offset); // 2nd vertex in the first triangle
    
}



void Quad::createQuad(
		FVector &a,
		FVector &b,
		FVector &c,
		FVector &d
){
    buildTriangle(a, b, c, vertecies, triangles);
    buildTriangle(a, c, d, vertecies, triangles);
}



//helper functions
FVector Quad::center(){
    FVector centerVec;
    int size = vertecies.Num();
    for (int i = 0; i < vertecies.Num(); i++)
    {
        centerVec += vertecies[i];
    }
    centerVec /= size;
    return centerVec;
}


bool Quad::isWithinQuad(FVector &other){
    if(vertecies.Num() != 6){
        return false;
    }

    //TODO: Prüfen ob der punkt mit dem skalarprodukt der normalen auf der ebene liegt
    //und dann überlegen wie man prüft das er in bounds der ebene ist



    //von oben einmal
    //von der seite quasi zweimal?
    FVector &a = vertecies[0];
    FVector &b = vertecies[1];
    FVector &c = vertecies[2];
    FVector &d = vertecies[5];

    return isWithinTriangle(other, a, b, c) || isWithinTriangle(other, b, c, d);
}

bool Quad::isWithinTriangle(FVector &target, FVector &a, FVector &b, FVector &c){
    FVector normal = FVector::CrossProduct(b - a, c - a);
    // Ex: = A + r (B-A) + s(C-A)
    // Ex: = n * ((x,y,z) - Q) = 0 //Q auf ebene
    // wenn das skalarprodukt zweier vektoren = 0 ergibt sind sie orthogonal zu einenander
    // wenn das skalarprodukt zweier vektoren |1| ergibt sind sie paralell zu einander
    // Ex: = n * (x - q1, y - q2, z - q3) = 0
    // Ex: = n1 (x-q1) + n2 * (y-q2) + n3 * (z-q3) = 0
    float x = target.X;
    float y = target.Y;
    float z = target.Z;

    //hier: Q auf ebene = a
    float dotproductEx = (normal.X * (x - a.X)) +
                        (normal.Y * (y - a.Y)) +
                        (normal.Z * (z - a.Z));

    float dotProductAbs = std::abs(dotproductEx);

    float epsilon = 0.01f;
    if (dotProductAbs <= 0 + epsilon){
        //distanzen messen
        // |a| = sqrt(a.x * a.x + a.y * a.y + a.z * a.z)
        // a * a = (a.x * a.x + a.y * a.y + a.z * a.z)
        return isInRangeOfAB(a, b, target) && 
                isInRangeOfAB(b, c, target) && 
                isInRangeOfAB(c, a, target);
    }
    return false;
}

/// @brief checks if the distance between a and b is greater than a compare and b compare
/// which ensures that the vector is maybe within the triangle. If each edge is true, the vector is within
/// the triangle
/// @param a start
/// @param b end
/// @param compare compare point
/// @return is in range of a to b
bool Quad::isInRangeOfAB(FVector &a, FVector &b, FVector &compare){
    FVector ab = b - a;
    FVector ac = compare - a;
    FVector bc = compare - b;
    // länge vektor:
    // |a| = sqrt(a.x * a.x + a.y * a.y + a.z * a.z) //euklid distance
    // a * a = (a.x * a.x + a.y * a.y + a.z * a.z)   //quadratic distance

    float lengthAb = (ab.X * ab.X) + (ab.Y * ab.Y) + (ab.Z * ab.Z);
    float lengthAc = (ac.X * ac.X) + (ac.Y * ac.Y) + (ac.Z * ac.Z);

    //between 1 of 2 ok
    if(lengthAb > lengthAc){
        float lengthBc = (bc.X * bc.X) + (bc.Y * bc.Y) + (bc.Z * bc.Z);

        //between 2 of 2 ok
        if(lengthAb > lengthBc){
            return true;
        }
    }
    return false;
}

bool Quad::doesHit(
    int xCheck,
    int yCheck,
    int xA,
    int yA,
    int xB,
    int yB
){

    int xNormal = -1 * yCheck;
    int yNormal = xCheck;

    // wenn beide vektoren auf einer seite liegen 
    // ist es tangential
    // ansonsten wird es geschnitten

    // (vn dot v1) * (vn dot v2) >= 0 => tangential
    // (vn dot v1) * (vn dot v2) < 0 => schnitt
    if(dotProduct(xNormal, yNormal, xA, yA) * dotProduct(xNormal, yNormal, xB, yB) <= 0){
        return true;
    }
    return false;
}

float Quad::dotProduct(
    int x0,
    int y0,
    int x1, 
    int y1
){
    return (x0 * x1) + (y0 * y1);
}
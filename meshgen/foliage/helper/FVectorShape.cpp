// Fill out your copyright notice in the Description page of Project Settings.


#include "FVectorShape.h"
#include "p2/meshgen/MeshData.h"
#include "p2/meshgen/generation/bezierCurve.h"
#include "GrahamScan.h"
#include "p2/util/TVector.h"

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
/// @param mat some matrix
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

void FVectorShape::push_back(std::vector<FVector> &other){
    for (int i = 0; i < other.size(); i++){
        vec.push_back(other[i]);
    }
}


/// @brief Auto joins the previous vertecies added without bricking the illumnation / shading
/// @param other mesh data to append smoothley to
void FVectorShape::joinMeshData(MeshData &other){
    other.appendVertecies(vec);
}

/// @brief creates a double sided mesh from own shape and center of mass
/// @return shape Mesh data, double sided, with calculated normals done!
MeshData FVectorShape::createDoubleSidedMesh(){
    MeshData mesh;
    
    //center of mass
    FVector center = calculateCenter();

    // connect triangles to center
    for (int i = 0; i < vec.size(); i++){
        //create double sided triangle
        int nextIndex = (i + 1) % vec.size();
        FVector &current = vec[i];
        FVector &next = vec[nextIndex];
        mesh.appendDoublesided(center, current, next);
    }

    mesh.calculateNormals();
    return mesh;
}


/// @brief closes mesh at center: clockwise means up, counter clockwise is down dir (bool flag)
/// @param clockWise clockwise means up, counter clockwise is down dir (bool flag)
/// @return meshdata, with calculated normals done!
MeshData FVectorShape::closeMeshAtCenter(bool clockWise){
    MeshData mesh;
    
    //center of mass
    FVector center = calculateCenter();
    mesh.closeMeshAtCenter(center, vec, clockWise);
    mesh.calculateNormals();
    return mesh;

}


/// @brief calculates the center of all vertecies
/// @return center
FVector FVectorShape::calculateCenter(){
    FVector center;
    for (int i = 0; i < vec.size(); i++){
        center += vec[i];
    }
    center /= vec.size();
    return center;
}

/// @brief creates vertecies in mmatrix represantation
/// @param output 
void FVectorShape::copyVertecies(std::vector<MMatrix> &output){
    for (int i = 0; i < vec.size(); i++){
        MMatrix mat;
        mat.setTranslation(vec[i]);
        output.push_back(mat);
    }
}




/**
 * new rand function for rocks
 */
void FVectorShape::randomizeVertecies(int maxdistance){

    FVector center = calculateCenter();
    for (int i = 0; i < vec.size(); i++)
    {
        FVector &current = vec[i];
        FVector dir = center - current;
        dir = dir.GetSafeNormal();

        current += dir * FVectorUtil::randomNumber(-maxdistance, maxdistance);
    }
}




/// @brief will smooth the shape with a detail of avg distance * 10
void FVectorShape::smoothWithBezier(){
    //distance on average
    if(vec.size() == 0){
        return;
    }
    float distance = 0.0f;
    for (int i = 0; i < vec.size(); i++){
        int nextIndex = (i + 1) % vec.size();
        distance += FVector::Dist(vec[i], vec[nextIndex]);
    }
    distance /= vec.size();

    //say 4 for example
    smoothWithBezier(distance / 10);
}

void FVectorShape::smoothWithBezier(int detailStep){
    if(vec.size() == 0){
        return;
    }

    std::vector<FVector> output;
    bezierCurve curve;
    curve.calculatecurve(
        vec,
        output,
        detailStep // einheits value
    );

    vec = output;
}



int FVectorShape::vertexCount(){
    return vec.size();
}



void FVectorShape::keepVertexCountFromFront(int countLimit){
    if(vec.size() == 0){
        return;
    }
    if(countLimit >= vec.size()){
        return;
    }

    vec.erase(vec.begin() + countLimit, vec.end());
}




/// @brief moves all vertecies so the pivot is the center
void FVectorShape::makeCenterPivot(){
    FVector centerToZero = -1 * calculateCenter();
    if(centerToZero.Size() == 0.0f){
        return;
    }

    MMatrix moveToCenter;
    moveToCenter.setTranslation(centerToZero);
    moveVerteciesWith(moveToCenter);
}




/**
 * 
 * 
 * --- helper functions for terrain ---
 * 
*/

std::vector<FVector> FVectorShape::vectorCopy(){
    return vec;
}

/// @brief adds a circle to the vector in CLOCKWISE ORDER (Important for mesh gen) PIVOT AT CENTER XY
/// regardless of the 
/// current vec, it gets pushed back
/// @param radius of the circle
/// @param detail detail of vertecies around the circle (4,8, 10 vertecies, etc) --> min: 4, is forced
void FVectorShape::createCircleShape(int radius, int detail){
    detail = std::abs(detail);
    if(detail < 4){
        detail = 4;
    }

    FVector baseVector(radius, 0, 0);
    int part = (360 / detail) * -1; //CLOCK WISE FOR CORRECT TRIANGLE WINDING ORDER
    for (int i = 0; i < detail; i++)
    {
        MMatrix rot;
        rot.yawRadAdd(MMatrix::degToRadian(part * i));
        FVector current = rot * baseVector;
        vec.push_back(current);
    }

}




void FVectorShape::createCircleShapeOnPitchRotation(int radius, int detail){
    detail = std::abs(detail);
    if(detail < 4){
        detail = 4;
    }

    FVector baseVector(0, 0, radius);
    float part = -360.0f / detail; 
    for (int i = 0; i < detail; i++)
    {
        MMatrix rot;
        rot.pitchRadAdd(MMatrix::degToRadian(part * i));
        FVector current = rot * baseVector;
        vec.push_back(current);
    }

}

void FVectorShape::createHalfCircleShapeOnPitchRotation(int radius, int detail){
    detail = std::abs(detail);
    if(detail < 4){
        detail = 4;
    }

    FVector baseVector(0, 0, radius);
    //int part = ((180 / detail) * -1); //0,90,180,270,360
    float part = -180.0f / detail;
    for (int i = 0; i <= detail; i++)
    {
        MMatrix rot;
        rot.pitchRadAdd(MMatrix::degToRadian(part * i));
        FVector current = rot * baseVector;
        vec.push_back(current);
    }
}


/// @brief adds a circle to the vector in CLOCKWISE ORDER PIVOT AT CENTER XY
/// (Important for mesh gen) regardless of the 
/// current vec, it gets pushed back
/// @param radius of the circle
/// @param detail detail of vertecies around the circle (4,8, 10 vertecies, etc) --> min: 4, is forced
void FVectorShape::createSpikedCircleShape(int radius, int radiusInner, int detail){
    detail = std::abs(detail);
    if(detail < 4){
        detail = 4;
    }

    FVector baseVectorOuter(radius, 0, 0);
    int part = (360 / detail) * -1; //CLOCK WISE FOR CORRECT TRIANGLE WINDING ORDER

    MMatrix moveInner;
    moveInner.yawRadAdd(MMatrix::degToRadian(part / 2)); //offset nach innen drehen sodass zacken entstehen
    FVector baseVectorInner(radiusInner, 0, 0);
    baseVectorInner = moveInner * baseVectorInner;

    for (int i = 0; i < detail; i++)
    {
        MMatrix rot;
        rot.yawRadAdd(MMatrix::degToRadian(part * i));
        FVector currentOuter = rot * baseVectorOuter;
        FVector currentInner = rot * baseVectorInner;
        vec.push_back(currentOuter);
        vec.push_back(currentInner);
    }

}

/// @brief creates a quad and appends it to the vertecies vector, PIVOT AT CENTER XY
/// @param sizeHalf size half of one side
void FVectorShape::createQuadShape(int sizeTotal){
    std::vector<FVector> vecQuad = MeshData::create2DQuadVertecies(sizeTotal, sizeTotal);
    int sizehalf = sizeTotal / 2;
    MMatrix tocenterMat;
    tocenterMat.setTranslation(-sizehalf, -sizehalf, 0);
    for (int i = 0; i < vecQuad.size(); i++)
    {
        FVector result = tocenterMat * vecQuad[i];
        vec.push_back(result);
    }
}

/// @brief creates a random shape from a circle base and smooths it with an bezier curve
/// @param sizeXYMax size of shape initially
/// @param smoothStep smooth step of curve 
void FVectorShape::createRandomNewSmoothedShape(int sizeXYMax, int smoothStep){
    vec.clear();

    int radius = sizeXYMax / 2;
    createCircleShape(radius, 10);
    randomizeVertecies(sizeXYMax / 10);

    smoothWithBezier(smoothStep);
}


void FVectorShape::createRandomNewSmoothedShapeClamped(int sizeXYMax, int smoothStep){
    createRandomNewSmoothedShape(sizeXYMax, smoothStep);

    /*
    int limit = sizeXYMax - 1;
    for (int i = 0; i < vec.size(); i++)
    {
        FVector &current = vec[i];
        clampVector(current, 0, limit);
    }*/
}

/// @brief clamps the x and y value to a given min and max value
/// @param pos 
/// @param minValue 
/// @param maxValue 
void FVectorShape::clampVector(FVector &pos, int minValue, int maxValue){
    if(pos.X < minValue){
        pos.X = minValue;
    }
    if(pos.Y < minValue){
        pos.Y = minValue;
    }

    if(pos.X > maxValue){
        pos.X = maxValue;
    }
    if(pos.Y > maxValue){
        pos.Y = maxValue;
    }
}

/// @brief makes all coordinates have floored values (integer)
void FVectorShape::floorAllCoordinateValues(){
    for (int i = 0; i < vec.size(); i++){
        FVector &current = vec[i];
        current.X = std::floor(current.X);
        current.Y = std::floor(current.Y);
        current.Z = std::floor(current.Z);
    }
}


/// @brief sorts the vertecies to be sorted in x direction, then y if needed
void FVectorShape::sortVerteciesOnXAxis(){
    try {
        std::sort(vec.begin(), vec.end(), [](const FVector &a, const FVector &b) {
            if(a.X == b.X){
                return a.Y < b.Y;
            }
            return a.X < b.X;
        });
    }
    catch (const std::exception &e) {
        DebugHelper::logMessage("error in sorting fvectorshape on xaxis");
    }
}



MeshData FVectorShape::createSphere(int radius, int detail, bool faceOutside){
    MeshData outMeshData;
    detail = std::abs(detail);
    if(detail < 8){
        detail = 8;
    }

    int part = ((360) / detail);
    if(faceOutside){
        part *= -1;
    }

    FVectorShape newShape;
    newShape.createHalfCircleShapeOnPitchRotation(radius, detail); //around pitch
    for (int i = 0; i < detail; i++)
    {
        MMatrix rotatorMat;
        rotatorMat.yawRadAdd(MMatrix::degToRadian(part * i));

        FVectorShape copy(newShape, rotatorMat);
        outMeshData.appendVertecies(copy.vec); // face inside
    }
    outMeshData.appendVertecies(newShape.vec); //last closing


    outMeshData.calculateNormals(); //checkup needed



    return outMeshData;
}



///@brief creates a half circle shape, open in the opposite direction of the passed normal
/// and aligned its rotation to it
/// extraOffset - create an extra vertex offset for example
/// pass in the fullCircleDetail to set the number of vertecies on the outer circle
/// which is speratly saved in the outlineshape vector passed in
MeshData FVectorShape::createHalfSphereCuttedOff(
    int radius, 
    int fullCircleDetail, 
    bool faceOutside,
    FVector normalDir,
    std::vector<FVector> &outlineShape,
    MMatrix extraOffset
){

    int halfDetail = fullCircleDetail / 2;
    MeshData outMeshData;
    halfDetail = std::abs(halfDetail);
    if(halfDetail < 8){
        halfDetail = 8;
    }
    
    MMatrix rotateHalfSphere = MMatrix::createRotatorFrom(normalDir); //eindrehen der punkte
    //MMatrix translateAndRotate = extraOffset * rotateHalfSphere; //M = T * R <-- lese richtung

    float part = ((180.0f) / halfDetail);
    if(faceOutside){
        part *= -1.0f;
    }

    FVectorShape newShape;
    newShape.createHalfCircleShapeOnPitchRotation(radius, halfDetail); //around pitch
    for (int i = 0; i < halfDetail; i++)
    {
        MMatrix rotatorMat;
        rotatorMat.yawRadAdd(MMatrix::degToRadian(90)); //create proper x axis alignment because pitch rot is 90 off!
        rotatorMat.yawRadAdd(MMatrix::degToRadian(part * i));

        FVectorShape copy(newShape, rotatorMat);
        outMeshData.appendVertecies(copy.vec); // face inside

    }


    //rotate half sphere
    outMeshData.transformAllVertecies(rotateHalfSphere);
    outMeshData.transformAllVertecies(extraOffset);
    outMeshData.calculateNormals(); //checkup needed



    //create outline rotated as needed in dir of normal
    FVectorShape circleOutliner;
    circleOutliner.createCircleShapeOnPitchRotation(radius, fullCircleDetail);

    MMatrix turn90;
    turn90.yawRadAdd(MMatrix::degToRadian(90)); // create proper x axis alignment because pitch rot is 90 off!

    circleOutliner.moveVerteciesWith(turn90);
    circleOutliner.moveVerteciesWith(rotateHalfSphere);
    circleOutliner.moveVerteciesWith(extraOffset);
    outlineShape = circleOutliner.vec;

    return outMeshData;

}

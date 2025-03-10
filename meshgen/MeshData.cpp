// Fill out your copyright notice in the Description page of Project Settings.


#include "p2/meshgen/MeshData.h"
#include "p2/entities/customIk/MMatrix.h"
#include "p2/DebugHelper.h"
#include "p2/meshgen/foliage/helper/GrahamScan.h"
#include "p2/meshgen/foliage/helper/FVectorShape.h"
#include "p2/meshgen/foliage/helper/ParallellShapeMerger.h"
#include "p2/gameStart/assetEnums/materialEnum.h"
#include "p2/meshgen/foliage/helper/baryCentricInterpolator.h"
#include "p2/DebugHelper.h"

#include <algorithm>
#include <set>

MeshData::MeshData()
{
}


/// @brief creates the mesh from triangles and vertecies AND calculates the normals!
/// @param verteciesIn 
/// @param trianglesIn 
MeshData::MeshData(TArray<FVector> &&verteciesIn, TArray<int> &&trianglesIn){
    setVertecies(MoveTemp(verteciesIn));
    setTriangles(MoveTemp(trianglesIn));
    calculateNormals();
}

void MeshData::rebuild(TArray<FVector> &&verteciesIn, TArray<int> &&trianglesIn){
    clearMesh();
    setVertecies(MoveTemp(verteciesIn));
    setTriangles(MoveTemp(trianglesIn));
    calculateNormals();
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

        materialPreferred = other.materialPreferred;
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
    
    
    clearNormals();
    normals.SetNum(vertecies.Num());
    for (int i = 0; i < triangles.Num() - 3; i += 3) {
        int32 Index0 = triangles[i];
        int32 Index1 = triangles[i + 1];
        int32 Index2 = triangles[i + 2];

        FVector Edge1 = vertecies[Index1] - vertecies[Index0];
        FVector Edge2 = vertecies[Index2] - vertecies[Index0];
        FVector Normal = FVector::CrossProduct(Edge1, Edge2).GetSafeNormal();

        normals[Index0] = Normal;
        normals[Index1] = Normal;
        normals[Index2] = Normal;
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

/// @brief join another mesh, vertecies add, triangles added with offset added to index
/// all data will be COPIED, regardless of duplicated vertecies, normals and triangles
/// use appendEfficent(Meshdata) to get more data efficent results
/// but with computational overhead!
/// @param other 
void MeshData::append(MeshData &other)
{
    TArray<FVector> &verteciesRef = other.getVerteciesRef();
    TArray<int32> &trianglesRef = other.getTrianglesRef();
    TArray<FVector> &normalsRef = other.getNormalsRef();
    join(verteciesRef, trianglesRef, normalsRef);

    updateBoundsIfNeeded();
}

void MeshData::join(TArray<FVector> &verteciesRef, TArray<int32> &trianglesRef, TArray<FVector> &normalsin){
    int triangleOffset = vertecies.Num(); //beim vertex count offset starten!

    //copy triangles, apply offset
    for (int i = 0; i < trianglesRef.Num(); i++){
        int32 copy = trianglesRef[i];
        copy += triangleOffset;
        //--> offset index weil buffer sich verschiebt insgesamt um size des aktuellen
        //    vertex buffers
        triangles.Add(copy);
    }

    //copy vertecies
    for(int i = 0; i < verteciesRef.Num(); i++){
        FVector &ref = verteciesRef[i];
        vertecies.Add(ref);
    }

    //copy normals
    for(int i = 0; i < normalsin.Num(); i++){
        FVector &ref = normalsin[i];
        normals.Add(ref);
    }
}



/*

---- manual adding section ----

*/
void MeshData::append(
    FVector &a, 
    FVector &b, 
    FVector &c
){
    TArray<FVector> _vertecies;
    TArray<int32> _triangles;
    buildTriangle(a, b, c, _vertecies, _triangles);
    MeshData appendMesh(
        MoveTemp(_vertecies),
        MoveTemp(_triangles)
    );
    append(appendMesh);
}


void MeshData::append(
    FVector &a, 
    FVector &b, 
    FVector &c,
    FVector &d
){
    append(a, b, c);
    append(a, c, d);
}

void MeshData::appendDoublesided(
    FVector &a, 
    FVector &b, 
    FVector &c
){
    /*
    1 2
    0 
    */
   //kann nicht weiter vereinfacht werden damit jene duplikat vertecies auch andere normalen haben
    append(a, b, c); 
    append(a, c, b); 
}

void MeshData::appendDoublesided(
    FVector &a,
    FVector &b,
    FVector &c,
    FVector &d
){
    /*
    b c
    a d
    */
    appendDoublesided(a, b, c);
    appendDoublesided(a, c, d);
}


void MeshData::appendDoubleSidedTriangleBuffer(
    std::vector<FVector> &buffer
){  
    if(buffer.size() < 3){
        return;
    }

    for (int i = 0; i < buffer.size() - 3; i += 3)
    {
        appendDoublesided(
            buffer[i],
            buffer[i + 1],
            buffer[i + 2]
        );
    }
}




void MeshData::buildTriangle(
    FVector &a, 
    FVector &b, 
    FVector &c,
    TArray<FVector> &output,
    TArray<int32> &trianglesOutput
){
    //add vertecies
    /*
    output.Add(a);
    output.Add(b);
    output.Add(c);

    //add triangles
    int32 offset = trianglesOutput.Num();
    trianglesOutput.Add(0 + offset); // 0th vertex in the first triangle
    trianglesOutput.Add(1 + offset); // 1st vertex in the first triangle
    trianglesOutput.Add(2 + offset); // 2nd vertex in the first triangle*/

    

    //add triangles (new more readable)
    int32 offset = output.Num();
    trianglesOutput.Add(0 + offset); // 0th vertex in the first triangle
    trianglesOutput.Add(1 + offset); // 1st vertex in the first triangle
    trianglesOutput.Add(2 + offset); // 2nd vertex in the first triangle

    output.Add(a);
    output.Add(b);
    output.Add(c);
}



/// @brief offsets all vertecies in a given direction
/// @param offset 
void MeshData::offsetAllvertecies(FVector &offset){
    for (int i = 0; i < vertecies.Num(); i++){
        vertecies[i] += offset;
    }

    updateBoundsIfNeeded();
}

/// @brief transforms all vertecies with a given matrix
/// @param other 
void MeshData::transformAllVertecies(MMatrix &other){

    //matrix für vertecies: M
    for (int i = 0; i < vertecies.Num(); i++){
        vertecies[i] = other * vertecies[i];
    }

    //matrix für normalen: (M^-1)^T !!!! NICHT VERGESSEN!
    MMatrix M_inverse = other.createInverse();
    M_inverse.transpose();
    for (int i = 0; i < normals.Num(); i++){
        normals[i] = M_inverse * normals[i];
    }

    updateBoundsIfNeeded();
}





/// @brief appends the vertecies to the meshbuffer and creates triangles with n latest vertecies
/// to connect each vertex of the passed vector (allows for smooth 
/// shading and not duplicating vertecies)
/// caution: vec.size() should not be greater than the current vertex count, unless
/// this mesh data vertexcount is 0
/// will connect to the latest vec.size() vertecies, gaps cannot be detected
/// @param vec vector to connect, expected in some correct order 

void MeshData::appendVertecies(std::vector<FVector> &vec){
    if(vec.size() <= 0){
        return;
    }
    if(vertecies.Num() == 0){
        for (int i = 0; i < vec.size(); i++)
        {
            vertecies.Add(vec[i]);
        }
        return;
    }
    

    //soll sich an die letzten vertecies anheften
    //aber ohne vertecies zu duplizieren

    int32 startingIndex = vertecies.Num() - vec.size(); // 0
    if(vertecies.Num() < vec.size()){
        fillUpMissingVertecies(startingIndex); //difference abs
        startingIndex = 0;
    }

    int32 offset = vertecies.Num(); //stimmt so, getestet
    for (int i = 0; i < vec.size(); i++)
    {
        vertecies.Add(vec[i]);
    }

    //für jeden vertex den man hinzugefügt hat, aus 3 (+2) dreiecke basteln
    for (int i = 0; i < vec.size(); i++){

        
        // so.
        /*
        1 2
        0 3
        */
        // 1 triangle
        if(true){
            //das hier sieht eher nicht aus wie die 012 023 winding order, aber sie ist korrekt
            //nach aussen gedreht
            int next = (i + 1) % vec.size();
            triangles.Add(startingIndex + i); //lower(0)
            triangles.Add(startingIndex + next); //lower(3)
            triangles.Add(offset + i); //upper(1)
            
            //2 triangle
            triangles.Add(startingIndex + next); //lower (3)
            triangles.Add(offset + next); //upper(2)
            triangles.Add(offset + i); //upper (1)
        }
        
    }

    updateBoundsIfNeeded();
}

/**
 * caution: this function is not tested
 */
/// @brief will fill up vertecies between the last two ones
/// @param count 
void MeshData::fillUpMissingVertecies(int count){
    if(count <= 0){
        return;
    }
    int sizeOfVertexbuffer = vertecies.Num();
    count = std::abs(count);
    if(sizeOfVertexbuffer == 1){
        FVector copy = vertecies[0];
        for (int i = 0; i < count; i++)
        {
            vertecies.Add(copy);
        }
        return;
    }

    //DebugHelper::logMessage("meshData_debug inserted vertecies");

    FVector last = vertecies[sizeOfVertexbuffer - 1];
    FVector prev = vertecies[sizeOfVertexbuffer - 2];
    FVector connect = last - prev; //AB = B - A
    connect /= count;
    vertecies.RemoveAt(vertecies.Num() - 1);
    for (int i = 0; i < count; i++)
    {
        FVector inner = prev + connect * i;
        vertecies.Add(inner);
    }
    vertecies.Add(last);

    //fix triangle buffer offset because of insert
    int vertexIndexTargeted = sizeOfVertexbuffer - 1;
    for (int i = 0; i < triangles.Num(); i++)
    {
        if(triangles[i] == vertexIndexTargeted){
            triangles[i] = vertecies.Num() - 1;
        }
    }
}



void MeshData::closeMeshAtCenter(FVector &center, std::vector<FVector> &vec, bool clockWise){
    //vertecies.Add(center); //jetzt offset index valid
    for (int i = 0; i < vec.size(); i++){
        vertecies.Add(vec[i]);
    }
    closeMeshAtCenter(center, vec.size(), clockWise);
    return;

}

void MeshData::closeMeshAtCenter(FVector &center, int bufferSizeToConnect, bool clockWise){
    int limit = bufferSizeToConnect; //vertex count um an mitte zu binden
    if(limit > vertecies.Num()){
        limit = vertecies.Num();
    }
    int offset = vertecies.Num() - bufferSizeToConnect;
    if(offset < 0){
        offset = 0;
    }
    vertecies.Add(center);

    for (int i = 0; i < limit; i++)
    {
        int next = (i + 1) % limit;
        if(clockWise){
            /*
            1 2
            0
            */
            triangles.Add(offset); //0
            triangles.Add(offset + i); //1
            triangles.Add(offset + next); //2
        }else{
            /*
            2 1
            0
            */
           triangles.Add(offset); //0
           triangles.Add(offset + next); //2
           triangles.Add(offset + i); //1
        }
    }
}












/**
 * --- helper function ---
 */
/// @brief finds the closest index to a vertex, -1 if the vertex buffer is clear
/// @param vertex position to find
/// @return index in vertecies array
int MeshData::findClosestIndexTo(FVector &vertex){
    if(vertecies.Num() == 0){
        return -1;
    }
    int closestIndex = 0;
    float dist = FVector::Dist(vertex, vertecies[0]);

    for (int i = 1; i < vertecies.Num(); i++){
        float newDist = FVector::Dist(vertex, vertecies[i]);
        if(newDist < dist){ //debug keep distance
            dist = newDist;
            closestIndex = i;
        }
    }
    return closestIndex;
}

bool MeshData::isCloseSame(FVector &a, int index){
    if(index < 0 || index >= vertecies.Num()){
        return false;
    }
    return isCloseSame(a, vertecies[index]);
}

bool MeshData::isCloseSame(FVector &a, FVector &b){
    return std::abs(a.X - b.X) <= EPSILON &&
           std::abs(a.Y - b.Y) <= EPSILON &&
           std::abs(a.Z - b.Z) <= EPSILON;
}



int MeshData::findClosestIndexToAndAvoid(FVector &vertex, int indexAvoid){
    std::vector<int> vec = {indexAvoid};
    return findClosestIndexToAndAvoid(vertex, vec);
}

int MeshData::findClosestIndexToAndAvoid(FVector &vertex, std::vector<int> &avoid){
    if(vertecies.Num() == 0){
        return -1;
    }
    int closestIndex = 0;
    float dist = FVector::Dist(vertex, vertecies[0]);

    for (int i = 1; i < vertecies.Num(); i++){

        if(!contains(avoid, i)){
            float newDist = FVector::Dist(vertex, vertecies[i]);
            if(newDist < dist){ //debug keep distance
                dist = newDist;
                closestIndex = i;
            }
        }
    }
    return closestIndex;
}






/// @brief creates a vertex buffer sample of 0123 clockwise dir on XY Pane
/// @param xMax xMax size
/// @param yMax yMax size
/// @return starting from (0,0,0) 0123 clockwise buffer quad
std::vector<FVector> MeshData::create2DQuadVertecies(int xMax, int yMax){
    std::vector<FVector> shape;
    shape.push_back(FVector(0,0,0));
    shape.push_back(FVector(0,yMax,0));
    shape.push_back(FVector(xMax,yMax,0));
    shape.push_back(FVector(xMax,0,0));
    return shape;
}


/**
 * 
 * is tested!
 * 
 */

void MeshData::appendEfficent(
    FVector &a, 
    FVector &b, 
    FVector &c
){
    int indexA = findClosestIndexTo(a);
    int indexB = findClosestIndexTo(b);
    int indexC = findClosestIndexTo(c);

    int debugEfficentAdded = 3;

    //add if not found correctly
    if(!isCloseSame(a, indexA)){
        vertecies.Add(a);
        indexA = vertecies.Num() - 1; //0
        debugEfficentAdded--;
    }
    if(!isCloseSame(b, indexB)){
        vertecies.Add(b);
        indexB = vertecies.Num() - 1; //1
        debugEfficentAdded--;
    }
    if(!isCloseSame(c, indexC)){
        vertecies.Add(c);
        indexC = vertecies.Num() - 1; //2
        debugEfficentAdded--;
    }
    //add to triangle buffer
    triangles.Add(indexA);
    triangles.Add(indexB);
    triangles.Add(indexC);

    if(debugEfficentAdded > 0 && false){
        FString message = FString::Printf(TEXT("meshdata efficentAdded %d"), debugEfficentAdded);
        DebugHelper::logMessage(message);
    }

    updateBoundsIfNeeded();
}


void MeshData::appendEfficent(
    FVector &a, 
    FVector &b, 
    FVector &c,
    FVector &d
){
    appendEfficent(a, b, c);
    appendEfficent(a, c, d);
}



/// @brief appends all mesh data, eliminates duplicated vertecies,
/// and recalculates all normals
/// @param other some mesh data
void MeshData::appendEfficent(MeshData &other){

    TArray<int32> &trianglesRef = other.getTrianglesRef();
    for (int i = 0; i < trianglesRef.Num() - 3; i += 3){ //-2 ? (no, -3 is correct)
        int32 v0 = trianglesRef[i];
        int32 v1 = trianglesRef[i + 1];
        int32 v2 = trianglesRef[i + 2];

        if(
            other.isValidVertexIndex(v0) &&
            other.isValidVertexIndex(v1) &&
            other.isValidVertexIndex(v2)
        ){
            FVector &v0vertex = other.vertecies[v0];
            FVector &v1vertex = other.vertecies[v1];
            FVector &v2vertex = other.vertecies[v2];

            appendEfficent(v0vertex, v1vertex, v2vertex);
        }
    }

    calculateNormals();
}



FVector MeshData::createNormal(int v0, int v1, int v2){
    if(isValidVertexIndex(v0) && isValidVertexIndex(v1) && isValidVertexIndex(v2)){
        FVector &a = vertecies[v0];
        FVector &b = vertecies[v1];
        FVector &c = vertecies[v2];

        FVector ab = b - a;
        FVector ac = c - a;
        return FVector::CrossProduct(ab, ac);
    }
    return FVector(0,0,0);
}





bool MeshData::isValidVertexIndex(int i){
    return i >= 0 && i < vertecies.Num();
}

bool MeshData::isValidVertexIndex(int i, int j, int n){
    return isValidVertexIndex(i) && isValidVertexIndex(j) && isValidVertexIndex(n);
}

bool MeshData::isValidTriangleIndex(int i){
    return i >= 0 && i < triangles.Num();
}

bool MeshData::isValidNormalIndex(int i){
    return i >= 0 && i < normals.Num();
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










/**
 * 
 * --- special split mesh function ---
 * 
 */
void MeshData::seperateMeshIntoAllTrianglesDoubleSided(std::vector<MeshData> &meshDataVectorOutput){
    for (int i = 0; i < triangles.Num() - 3; i += 3) {
        int32 Index0 = triangles[i];
        int32 Index1 = triangles[i + 1];
        int32 Index2 = triangles[i + 2];

        if(
            isValidVertexIndex(Index0, Index1, Index2)
        ){
            FVector &v0 = vertecies[Index0];
            FVector &v1 = vertecies[Index1];
            FVector &v2 = vertecies[Index2];
            MeshData newTriangleMesh;
            newTriangleMesh.appendDoublesided(v0, v1, v2); //normals will be calculated in the constructor
            meshDataVectorOutput.push_back(newTriangleMesh);
        }
    }
}



/// @brief will create all triangles possible to split and append it into the vector
/// traingles being lower than the min distance will not be added!
/// @param meshDataVectorOutput 
void MeshData::splitAllTrianglesInHalfAndSeperateMeshIntoAllTrianglesDoubleSided(
    std::vector<MeshData> &meshDataVectorOutput
){
    for (int i = 0; i < triangles.Num() - 3; i += 3) {
        int32 Index0 = triangles[i];
        int32 Index1 = triangles[i + 1];
        int32 Index2 = triangles[i + 2];

        FVector &v0 = vertecies[Index0];
        FVector &v1 = vertecies[Index1];
        FVector &v2 = vertecies[Index2];

        if(canSplit(v0,v1,v2)){
            FVector middle = v1 + 0.5f * (v2 - v1); // gx = A + r(B-A)

            MeshData newTriangleMeshA;
            newTriangleMeshA.appendDoublesided(v0, v1, middle);
            newTriangleMeshA.calculateNormals();
            meshDataVectorOutput.push_back(newTriangleMeshA);

            MeshData newTriangleMeshB;
            newTriangleMeshB.appendDoublesided(v0, middle, v2);
            newTriangleMeshB.calculateNormals();
            meshDataVectorOutput.push_back(newTriangleMeshB);
        }else{
            //DebugHelper::logMessage("DEBUGSPLIT could not split");
        }
    }

}

bool MeshData::canSplit(FVector &a, FVector &b, FVector &c){
    if(FVector::Dist(a,b) < MIN_SPLITDISTANCE){
        return false;
    }
    if(FVector::Dist(a,c) < MIN_SPLITDISTANCE){
        return false;
    }
    if(FVector::Dist(b,c) < MIN_SPLITDISTANCE){
        return false;
    }
    return true;
}


/// @brief calculates the center of the vertecies
/// @return center
FVector MeshData::center(){
    FVector zeroVec(0, 0, 0);
    if(vertecies.Num() <= 0){
        return zeroVec;
    }

    for (int i = 0; i < vertecies.Num(); i++){
        zeroVec += vertecies[i];
    }
    zeroVec /= vertecies.Num();
    return zeroVec;
}

/// @brief centers the mesh
void MeshData::centerMesh(){
    FVector thiscenter = center();
    for (int i = 0; i < vertecies.Num(); i++){
        vertecies[i] -= thiscenter;
    }
}



/**
 * 
 * -- mesh cutting helpers --
 * 
 */

//not tested
void MeshData::cutHoleWithInnerExtensionOfMesh(
    FVector &vertex, int radius
){

    //ERST cut, dann append(?)
    std::vector<FVector> outOfRangeVertecies;
    cutHole(vertex, radius, outOfRangeVertecies);
    if(outOfRangeVertecies.size() == 0){
        return;
    }

    //achtung: vertecies müssen durch 2 teilbar sein sodass der helbkreis korrekt
    //gespaltet wird (?)
    if(outOfRangeVertecies.size() % 2 != 0){
        FVector &ownBack = outOfRangeVertecies.back();
        outOfRangeVertecies.push_back(ownBack);
    }

    //find rotation of direction to rotate the halfsphere as needed
    FVector centerOfMesh = center();
    //FVector connect = centerOfMesh - vertex; //AB = B - A ---> vertex to center inside
    FVector connect(0, 0, 1);
    int indexClose = findClosestIndexTo(vertex);
    if (isValidNormalIndex(indexClose))
    {
        connect = normals[indexClose] * -1; //inward
    }

    std::vector<FVector> outlineOfHalfSphere; //online of open side!
    int detailPerCircle = outOfRangeVertecies.size();

    //fill missing detial (temporary solution)
    if(detailPerCircle < 8){
        int missing = 8 - detailPerCircle;
        int betweenExtra = missing / detailPerCircle;
        if(betweenExtra == 0){
            betweenExtra = 1;
        }
        betweenExtra += 1; //dass bei 1 extra, auch einer dazukommt.

        std::vector<FVector> modifiedOutOfRange;
        for (int i = 0; i < outOfRangeVertecies.size(); i++)
        {
            FVector &start = outOfRangeVertecies[i];
            FVector &end = outOfRangeVertecies[(i+1) % outOfRangeVertecies.size()];
            FVector connectScaled = end - start;
            connectScaled /= betweenExtra;
            for (int j = 0; j < betweenExtra; j++){
                FVector interpolated = start + connectScaled * j;
                modifiedOutOfRange.push_back(interpolated);
            }
        }
        outOfRangeVertecies = modifiedOutOfRange;
    }
    //fill missing detial (temporary solution) end


    MeshData sphere = FVectorShape::createHalfSphereCuttedOff(
        radius,
        detailPerCircle, //detail per full circle
        false, //false for face inside
        connect, // normal inwards to rotate half sphere / align with it
        outlineOfHalfSphere,
        MMatrix(vertex) // move to vertex pos
    );



    //mergen und anhängen den gefüllten kreis
    ParallellShapeMerger merger;
    merger.createParallelSortedShape(
        outlineOfHalfSphere,
        outOfRangeVertecies
    );
    std::vector<FVector> &bufferReference = merger.triangleBufferReference();
    appendDoubleSidedTriangleBuffer(bufferReference);



    //append sphere
    //appendEfficent(sphere);
    append(sphere);

    calculateNormals();
    
    
}



///@brief will cut a hole in a radius at a vertex position
///@param outOfRangeVertecies will save vertecies which were not cutted out
///but were connected to the cutted out vertecies
void MeshData::cutHole(FVector &vertex, int radius, std::vector<FVector> &outOfRangeVertecies) //connected out of range
{
    radius = std::abs(radius);
    int index = findClosestIndexTo(vertex);
    if (isValidVertexIndex(index))
    {
        FVector removedVertex = vertecies[index];

        //delete connected triangles, does change the triangle buffer but not the
        //vertex buffer
        //remove vertex by swapping with end and change the triangle buffer as needed
        //(update the indices in the triangle buffer)
        std::vector<int> connectedvertecies;
        removeVertex(index, connectedvertecies);

        int debugCount = 1;

        //cut recursivly until distance reached
        int i = 0;
        int size = connectedvertecies.size();
        while(i < size){
            int currentVertexIndex = connectedvertecies[i];
            if(isValidVertexIndex(currentVertexIndex)){
                FVector &currentVertex = vertecies[currentVertexIndex];
                float dist = FVector::Dist(currentVertex, removedVertex);

                if(dist < radius){
                    removeVertex(currentVertexIndex, connectedvertecies);
                    debugCount++;

                    //update size
                    size = connectedvertecies.size();
                }else{
                    //for appending some shape
                    outOfRangeVertecies.push_back(currentVertex);
                }
            }
            i++;
        }

        //FString message = FString::Printf(TEXT("debugremoved vertecies %d"), debugCount);
        //DebugHelper::logMessage(message);
    }
}

///@brief removes out a vertex from the mesh
void MeshData::removeVertex(int index){
    std::vector<int> ignored;
    removeVertex(index, ignored);
}

///@brief removes out a vertex from the mesh data by index and:
///@param connectedvertecies saves the connected vertecies defined by the triangle buffer
void MeshData::removeVertex(int index, std::vector<int>& connectedvertecies){

    if(isValidVertexIndex(index) && vertecies.Num() > 0){
        removeTrianglesInvolvedWith(index, connectedvertecies);

        //remove vertex by swapping with end and change the triangle buffer as needed
        int oldEnd = vertecies.Num() - 1;
        vertecies[index] = vertecies[oldEnd]; //index der removed wird nach hinten tauschen
        vertecies.Pop(); // pop back end which is in new pos now
        if(isValidNormalIndex(index) && isValidNormalIndex(oldEnd)){
            normals[index] = normals[oldEnd];
            normals.Pop();
        }

        //(update the indices in the triangle buffer because the vertex has changed)
        for (int i = 0; i < triangles.Num(); i++){
            if(triangles[i] == oldEnd){
                triangles[i] = index; //update weil swap
            }
        }

        //find old end in connected vertecies and replace the index
        for (int i = 0; i < connectedvertecies.size(); i++){
            int vertexIndexNow = connectedvertecies[i];
            if(oldEnd == vertexIndexNow){
                connectedvertecies[i] = index; //update weil swap
            }
        }
    }
}

///@brief removes all triangles from the triangle buffer by vertex index and
///@param connectedvertecies saves the connected vertecies for processing
void MeshData::removeTrianglesInvolvedWith(int vertexIndex, std::vector<int> &connectedvertecies){
    if(!isValidVertexIndex(vertexIndex)){
        return;
    }
    TArray<int32> triangleBufferCopy;
    for (int i = 0; i < triangles.Num() - 3; i += 3)
    {
        int32 v0 = triangles[i];
        int32 v1 = triangles[i+1];
        int32 v2 = triangles[i+2];

        bool v0ok = (v0 != vertexIndex);
        bool v1ok = (v1 != vertexIndex);
        bool v2ok = (v2 != vertexIndex);

        if(v0ok && v1ok && v2ok){
            triangleBufferCopy.Add(v0);
            triangleBufferCopy.Add(v1);
            triangleBufferCopy.Add(v2);
        }else{
            //dont copy the vertex which is removed
            if(v0ok && !contains(connectedvertecies, v0)){
                connectedvertecies.push_back(v0);
            }
            if(v1ok && !contains(connectedvertecies, v1)){
                connectedvertecies.push_back(v1);
            }
            if(v2ok && !contains(connectedvertecies, v2)){
                connectedvertecies.push_back(v2);
            }
            
        }
    }
    triangles = triangleBufferCopy;
}


bool MeshData::contains(std::vector<int> &ref, int index){
    for (int i = 0; i < ref.size(); i++){
        if(ref[i] == index){
            return true;
        }
    }
    return false;
}



/**
 * 
 * 
 * mesh deform no remove helper
 * 
 * 
 */
void MeshData::pushInwards(FVector &location, int radius, FVector scaleddirection){
    if(vertecies.Num() == 0){
        return;
    }

    radius = std::abs(radius);

    std::vector<int> connected;
    int index = findClosestIndexTo(location);
    if(!isValidVertexIndex(index)){
        return;
    }
    FVector foundLocation = vertecies[index];

    findConnectedVerteciesTo(index, connected);

    //recursivly find all connected vertecies from the triangle buffer
    int i = 0;
    int size = connected.size();
    while(i < size){
        if(i < connected.size()){
            int currentIndex = connected[i];
            if(isValidVertexIndex(currentIndex)){
                FVector &currentVertex = vertecies[currentIndex];
                float dist = FVector::Dist(currentVertex, foundLocation);
                if(dist < radius){
                    findConnectedVerteciesTo(currentIndex, connected);
                    size = connected.size();
                }
            }
        }

        i++;
    }


    // apply scaled offset direction
    for (int j = 0; j < connected.size(); j++)
    {
        int currentIndex = connected[j];
        if (isValidVertexIndex(currentIndex))
        {
            FVector &vertex = vertecies[currentIndex];
            if(isInsideBoundingbox(vertex)){
                vertex += scaleddirection;
            }
            
        }
    }

}

void MeshData::findConnectedVerteciesTo(int index, std::vector<int> &output){
    
    for (int i = 0; i < triangles.Num() - 3; i += 3)
    {
        int v0 = triangles[i];
        int v1 = triangles[i+1];
        int v2 = triangles[i+2];
        if(v0 == index || v1 == index || v2 == index){
            if (!contains(output, v0)){
                output.push_back(v0);
            }
            if (!contains(output, v1)){
                output.push_back(v1);
            }
            if (!contains(output, v2)){
                output.push_back(v2);
            }
        }
    }
}




/**
 * 
 * foliage helper
 * 
 */
materialEnum MeshData::targetMaterial(){
    return materialPreferred;
}
void MeshData::setTargetMaterial(materialEnum inMaterial){
    materialPreferred = inMaterial;
}



/// @brief generates the matricies to move an object to the vertex position
/// and rotate in look dir of normal. CAUTION: X axis is forward! The rotation block is
/// in yaw and pitch rotation relative to the XAxis! (1,0,0), orient your mesh accordingly
/// if you are using this methods to place a mesh on a surface for example!
/// @param output 
void MeshData::generateMatricesPerFaceAndLookDirOfNormal(
    std::vector<MMatrix> &output
){
    for (int i = 0; i < triangles.Num() - 3; i+= 3){
        int v0 = triangles[i];
        int v1 = triangles[i+1];
        int v2 = triangles[i+2];
        if(isValidVertexIndex(v0, v1, v2)){
            FVector &vertex0 = vertecies[v0];
            FVector &vertex1 = vertecies[v1];
            FVector &vertex2 = vertecies[v2];

            FVector v0v1 = vertex1 - vertex0; // AB = B - A;
            FVector v0v2 = vertex2 - vertex0;
            FVector normal = FVector::CrossProduct(v0v1, v0v2);
            normal = normal.GetSafeNormal();


            MMatrix rotator = MMatrix::createRotatorFrom(normal);
            //debug
            if(false){
                FVector axis(1, 0, 0);
                axis = rotator * axis;
                DebugHelper::logMessage("debug comparenormal", axis, normal); //works correct!
            }
            // debug end

            MMatrix translation;
            FVector center = (vertex0 + vertex1 + vertex2) / 3;
            translation.setTranslation(center);

            //translation.setTranslation(vertex0); //debug

            MMatrix TR = translation * rotator; //<-- lese richtung --
    
            output.push_back(TR);
        }
    }
}



void MeshData::generateMatricesPerFaceAndLookDirOfNormalInterpolated(
    std::vector<MMatrix> &output,
    int stepSize
){


    baryCentricInterpolator interpolator;
    for (int i = 0; i < triangles.Num() - 3; i+= 3){
        int v0 = triangles[i];
        int v1 = triangles[i+1];
        int v2 = triangles[i+2];
        if(isValidVertexIndex(v0, v1, v2)){
            FVector &vertex0 = vertecies[v0];
            FVector &vertex1 = vertecies[v1];
            FVector &vertex2 = vertecies[v2];

            
            interpolator.setup(vertex0, vertex1, vertex2);
            std::vector<MMatrix> newMatrices;
            interpolator.interpolateAllAsMatrixTransformToVertexAndLookOfNormal(
                stepSize,
                newMatrices
            );

            for (int j = 0; j < newMatrices.size(); j++){
                output.push_back(newMatrices[j]);
            }
        }
    }

}





/**
 * 
 * room mesh data helper 
 * 
 */

void MeshData::appendCube(
    FVector &a, 
    FVector &b,
    FVector &c,
    FVector &d,
    FVector &a1, 
    FVector &b1,
    FVector &c1,
    FVector &d1
){
    appendEfficent(a, d, c, b);
    appendEfficent(a1, b1, c1, d1);
    appendEfficent(b, b1, a1, a);
    appendEfficent(c, c1, b1, b);
    appendEfficent(d, d1, c1, c);
    appendEfficent(a, a1, d1, d);
    calculateNormals();
}

void MeshData::appendCube(
    FVector &a,
    FVector &b,
    FVector &c,
    FVector &d,
    FVector orthogonalDir
){
    FVector a1 = a + orthogonalDir;
    FVector b1 = b + orthogonalDir;
    FVector c1 = c + orthogonalDir;
    FVector d1 = d + orthogonalDir;
    appendCube(a, b, c, d, a1, b1, c1, d1);
}



int MeshData::verteciesNum(){
    return vertecies.Num();
}

void MeshData::updateBoundsIfNeeded(){
    bottomLeftBound = FVector(0, 0, 0);
    topRightBound = FVector(0, 0, 0);
    for (int i = 0; i < vertecies.Num(); i++)
    {
        updateBoundsIfNeeded(vertecies[i]);
    }
}


void MeshData::updateBoundsIfNeeded(FVector &other){
    if(other.X < bottomLeftBound.X){
        bottomLeftBound.X = other.X;
    }
    if(other.Y < bottomLeftBound.Y){
        bottomLeftBound.Y = other.Y;
    }
    if(other.Z < bottomLeftBound.Z){
        bottomLeftBound.Z = other.Z;
    }

    if(other.X > topRightBound.X){
        topRightBound.X = other.X;
    }
    if(other.Y > topRightBound.Y){
        topRightBound.Y = other.Y;
    }
    if(other.Z > topRightBound.Z){
        topRightBound.Z = other.Z;
    }
}

///@brief checks if a vertex is inside the bounding box, NOT allowing edge cases!
bool MeshData::isInsideBoundingbox(FVector &other){
    return other.X > bottomLeftBound.X &&
           other.Y > bottomLeftBound.Y &&
           other.Z > bottomLeftBound.Z &&
           other.X < topRightBound.X &&
           other.Y < topRightBound.Y &&
           other.Z < topRightBound.Z;
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreMinimal.h"
#include <cmath>
#include "Math/Rotator.h"
#include "MMatrix.h"

MMatrix::MMatrix()
{
    makeIdentity();
}

MMatrix::~MMatrix()
{

}



void MMatrix::makeIdentity(){
    for (int i = 0; i < 16; i++){
        array[i] = 0.0f;
    }
    array[0] = 1.0f;
    array[5] = 1.0f;
    array[10] = 1.0f;
    array[15] = 1.0f;
}


/// @brief resets the rotation back to einheits matrix but not adjusting the translation
void MMatrix::resetRotation(){
    for (int row = 0; row < 3; row++){
        for (int col = 0; col < 3; col++){
            int i = row * 4 + col; //obviously
            array[i] = 0.0f;
        }
    }
    //einheits matrix setzen
    array[0] = 1.0f;
    array[5] = 1.0f;
    array[10] = 1.0f;
    array[15] = 1.0f;


}

MMatrix::MMatrix(FVector &other){
    makeIdentity();
    setTranslation(other);
}

MMatrix::MMatrix(FRotator &other){
    makeIdentity();
    setRotation(other);
}

/// @brief copy constructor
/// @param other 
MMatrix::MMatrix(const MMatrix &other){
    makeIdentity();
    if (&other != this)
    {
        *this = other;
    }
}

//copy constructor
MMatrix& MMatrix::operator=(const MMatrix &other){
    if(this == &other){
        return *this;
    }

    //copy data
    for (int i = 0; i < size; i++){
        array[i] = other.array[i];
    }

    return *this;
}



void MMatrix::setTranslation(FVector &other){
    //copy to correct positions in array 
    //(0,1,2,3)
    //(-,-,-,7)
    //(-,-,-,11)
    //(-,-,-,-) //last row stays emtpy
    array[3] = other.X;
    array[7] = other.Y;
    array[11] = other.Z;
}

void MMatrix::setTranslation(float x, float y, float z){
    array[3] = x;
    array[7] = y;
    array[11] = z;
}

/// @brief will copy the translation of the given matrix
/// @param other other matrix to copy the translation from
void MMatrix::setTranslation(MMatrix &other){
    array[3] = other.array[3];
    array[7] = other.array[7];
    array[11] = other.array[11];
}

FVector MMatrix::getTranslation(){
    FVector out;
    out.X = array[3];
    out.Y = array[7];
    out.Z = array[11];
    return out;
}

void MMatrix::operator+=(MMatrix &other){

}



/// @brief multiply with another matrix
/// @param other 
void MMatrix::operator*=(MMatrix &other){
    MMatrix r = *this * other;
    *this = r;
}

/// @brief multiply and return result
/// @param other other matrix to append like: This * other 
/// @return returns a new matrix
MMatrix MMatrix::operator*(MMatrix &other){
    
    MMatrix result; // Temporary matrix to store the result

    for (int row = 0; row < 4; row++) { // Iterate through the rows of 'this' matrix
        for (int col = 0; col < 4; col++) { // Iterate through the columns of 'other' matrix
            float sum = 0.0f;
            for (int inner = 0; inner < 4; inner++) { // Perform dot product
                sum += array[row * 4 + inner] * other.array[inner * 4 + col];
            }
            result.array[row * 4 + col] = sum; // Store the result in the temporary matrix
        }
    }
    
    return result;
}





/// @brief multiply with a vector 
/// @param other 
/// @return 
FVector MMatrix::operator*(FVector &other){
    FVector resultVec;

    float row[] = {other.X, other.Y, other.Z, 1.0f};
    float resultRow[] = {0.0f, 0.0f, 0.0f};
    
    for (int thisRow = 0; thisRow < 3; thisRow++) //jede row dieser matrix
    {
        float result = 0.0f;
        for (int c = 0; c < 4; c++){
            result += array[thisRow * 4 + c] * row[c];
        }
        resultRow[thisRow] = result;
    }
    

    resultVec.X = resultRow[0];
    resultVec.Y = resultRow[1];
    resultVec.Z = resultRow[2];
    return resultVec;
}

FVector2D MMatrix::operator*(FVector2D &other){
    FVector d3Vec(other.X, other.Y, 0.0f);
    d3Vec = *this * d3Vec;
    return FVector2D(d3Vec.X, d3Vec.Y);
}



void MMatrix::operator+=(FVector &other){
    array[3] += other.X;
    array[7] += other.Y;
    array[11] += other.Z;
}
void MMatrix::operator-=(FVector &other){
    FVector copy = other * -1;
    *this += other;
}

/// @brief creates a string representation if the matrix with radian angles
/// @return FString by value representation of the matrix
FString MMatrix::asString(){
    FString output = "debug matrix: [";
    for (int i = 0; i < 16; i++)
    {
        output += FString::Printf(TEXT(" %.2f"), (array[i]));
        if(i % 4 == 3){
            output += "][";
        }
    }
    return output;
}





/// @brief util converts degrees to radian
/// @param deg deg input
/// @return 
float MMatrix::degToRadian(float deg){
    float angleInRadians = ((deg * M_PI) / 180.0); // Umwandlung in Bogenmaß
    return angleInRadians;
}

float MMatrix::radToDegree(float rad){
    float angleInDeg = ((rad * 180.0) / M_PI); // Umwandlung in Bogenmaß
    return angleInDeg;
}





void MMatrix::rollRad(float a){
    /*
    5  6
    9 10
    cos, -sin
    sin, cos
    */
    float cos = std::cos(a);
    float sin = std::sin(a);

    array[5] = cos;
    array[9] = sin;
    array[6] = sin * -1;
    array[10] = cos;
}





/// @brief rotate with y
/// @param a
void MMatrix::pitchRad(float a){
    /*
    0  2
    8 10
    cos, sin
    -sin, cos
    */
    float cos = std::cos(a);
    float sin = std::sin(a);
    array[0] = cos;
    array[10] = cos;
    array[2] = sin;
    array[8] = sin * -1;
}

void MMatrix::yawRad(float a){
    /*
    0  1
    4 5
    cos, -sin
    sin, cos
    */
    float cos = std::cos(a);
    float sin = std::sin(a);

    array[0] = cos;
    array[1] = sin * -1;
    array[5] = cos;
    array[4] = sin;
}

/**
 * 
 * ADD
 * 
 */


/// @brief rotate along X in radian
/// @param a 
void MMatrix::rollRadAdd(float a){
    if(a == 0.0f){
        return;
    }

    /*
    5  6
    9 10
    cos, -sin
    sin, cos
    */
    // Create a rotation matrix for rotation, then concatenate to not brick anything
    MMatrix rotMatrix;
    float cos = std::cos(a);
    float sin = std::sin(a);

    rotMatrix.array[5] = cos;
    rotMatrix.array[9] = sin;
    rotMatrix.array[6] = sin * -1;
    rotMatrix.array[10] = cos;

    
    rotate(rotMatrix);
   
}

/// @brief rotate with y
/// @param a radian
void MMatrix::pitchRadAdd(float a){
    if(a == 0.0f){
        return;
    }
    /*
    0  2
    8 10
    cos, sin
    -sin, cos
    */
    MMatrix rotMatrix;
    float cos = std::cos(a);
    float sin = std::sin(a);
    rotMatrix.array[0] = cos;
    rotMatrix.array[2] = sin;
    rotMatrix.array[8] = sin * -1;
    rotMatrix.array[10] = cos;
    
    rotate(rotMatrix);
    
}

void MMatrix::yawRadAdd(float a){
    if(a == 0.0f){
        return;
    }

    /*
    0  1
    4 5
    cos, -sin
    sin, cos
    */
    MMatrix rotMatrix;
    float cos = std::cos(a);
    float sin = std::sin(a);

    rotMatrix.array[0] = cos;
    rotMatrix.array[1] = sin * -1;
    rotMatrix.array[4] = sin;
    rotMatrix.array[5] = cos;

    rotate(rotMatrix);
}

/// @brief factory method to create a rotation around x, y and z
/// @param x x roll radian rotation
/// @param y y pitch radian rotation
/// @param z z yaw radian rotation
/// @return MMatrix which will rotate another matrix around the desired rotation when multiplying
MMatrix MMatrix::createRotatorFromRad(float x, float y, float z){

    MMatrix result;
    result.rollRadAdd(x);
    result.pitchRadAdd(y);
    result.yawRadAdd(z);

    //M = RX * RY * RZ 

    return result;
}






/// @brief factory method to create a rotation around x, y and z
/// @param x x roll degree rotation
/// @param y y pitch degree rotation
/// @param z z yaw degree rotation
/// @return MMatrix which will rotate another matrix around the desired rotation when multiplying
MMatrix MMatrix::createRotatorFromDeg(float x, float y, float z){
    x = MMatrix::degToRadian(x);
    y = MMatrix::degToRadian(y);
    z = MMatrix::degToRadian(z);
    return createRotatorFromRad(x, y, z);
}


MMatrix MMatrix::createRotatorFrom(FRotator &other){
    float x = MMatrix::degToRadian(other.Roll);
    float y = MMatrix::degToRadian(other.Pitch);
    float z = MMatrix::degToRadian(other.Yaw);

    /*
    //unreal reihenfolge scheinbar z-y-x
    MMatrix result;
    
    result.yawRadAdd(z);
    result.pitchRadAdd(y);
    result.rollRadAdd(x);

    return result;*/
    return createRotatorFromRad(x, y, z);
}


/// @brief creates the rotator from a given vector in pitch and yaw rotation! caution: both relative axis of measurement
/// are the x axis for yaw and pitch!! (1,0) (1,0)!!
/// @param other vector (direction) - will be normalized internally.
/// @return rotator matrix
MMatrix MMatrix::createRotatorFrom(FVector &other){
    FVector2D Xaxis(1.0f, 0); //default axis, x nach vorne schauend
    return createRotatorFrom(other, Xaxis, Xaxis); 
}


/// @brief 
/// @param other 
/// @param XAxis axis to measure yaw agains
/// @param ZAxis axis to measure pitch against
/// @return 
MMatrix MMatrix::createRotatorFrom(FVector &other, FVector2D XAxis, FVector2D ZAxis){
    FVector normalized = other.GetSafeNormal();

    //yaw angle
    FVector2D xydir(normalized.X, normalized.Y);
    float yawRad = signedAngleRadBetween(XAxis, xydir); //signiert voll umfänglich notwendig anders als pitch

    //idee: yaw angle raus nehmen damit der pitch korrekt projeziert ist! (das ist richtig so!)
    MMatrix removeYaw;
    removeYaw.yawRadAdd(-yawRad);
    normalized = removeYaw * normalized;
    normalized = normalized.GetSafeNormal();

    //pitch angle
    FVector2D xzdir(normalized.X, normalized.Z);
    float pitchRad = signedAngleRadBetween(ZAxis, xzdir) * -1.0f; //test (negieren. Aus. Gründen.) 

    


    MMatrix rotationMat;
    rotationMat.yawRadAdd(yawRad);
    rotationMat.pitchRadAdd(pitchRad);

    return rotationMat;
}



// ----- EXPERIMENTAL SECTION ------


MMatrix MMatrix::createRotatorFrom(
    FVector &other, 
    FVector2D XAxis, 
    FVector2D ZAxis,
    bool yawConstraint90
){
    FVector normalized = other.GetSafeNormal();

    //yaw angle
    FVector2D xydir(normalized.X, normalized.Y);
    float yawRad = signedAngleRadBetween(XAxis, xydir); //signiert voll umfänglich notwendig anders als pitch
    
    /**
     * wenn sich der zielpunkt nahe der 0 werte befindet,
     * ist der acos(0) = 90 degree, was seltsame
     * sprünge erzeugt.
     */
    bool noYaw = false;
    FVector xyProjection(other.X, other.Y, 0.0f);
    if (xyProjection.Size() < 0.1f){ //prevents weird rotations.
        yawRad = 0.0f;
        noYaw = true;
    }

    //idee: yaw angle raus nehmen damit der pitch korrekt projeziert ist! (das ist richtig so!)
    MMatrix removeYaw;
    removeYaw.yawRadAdd(-yawRad);
    normalized = removeYaw * normalized;
    normalized = normalized.GetSafeNormal();

    //pitch angle
    FVector2D xzdir(normalized.X, normalized.Z);
    float pitchRad = signedAngleRadBetween(ZAxis, xzdir) *-1.0f; // test (negieren. Aus. Gründen.)

    //is tested. 
    if(yawConstraint90 && !noYaw){
        float rad90 = M_PI / 2.0f;
    
        //makes yaw angle on the positive side and flips pitch instead!
        if (std::abs(yawRad) > rad90) {
            yawRad += (yawRad > 0.0f) ? -M_PI : M_PI;
            pitchRad *= -1.0f;
        }
        
        
    }

    MMatrix rotationMat;
    rotationMat.yawRadAdd(yawRad);
    rotationMat.pitchRadAdd(pitchRad);
    

    return rotationMat;
}


//debug
MMatrix MMatrix::createRotatorFrom(
    FVector &other, 
    FVector2D XAxis, 
    FVector2D ZAxis,
    bool yawConstraint90,
    float &outYaw,
    float &outPitch
){
    FVector normalized = other.GetSafeNormal();

    //yaw angle
    FVector2D xydir(normalized.X, normalized.Y);
    float yawRad = signedAngleRadBetween(XAxis, xydir); //signiert voll umfänglich notwendig anders als pitch
    
    /**
     * wenn sich der zielpunkt nahe der 0 werte befindet,
     * ist der acos(0) = 90 degree, was seltsame
     * sprünge erzeugt.
     */
    bool noYaw = false;
    FVector xyProjection(other.X, other.Y, 0.0f);
    if (false && xyProjection.Size() < 0.1f){ //prevents weird rotations.
        yawRad = 0.0f;
        noYaw = true;
    }

    //idee: yaw angle raus nehmen damit der pitch korrekt projeziert ist! (das ist richtig so!)
    MMatrix removeYaw;
    removeYaw.yawRadAdd(-yawRad);
    normalized = removeYaw * normalized;
    normalized = normalized.GetSafeNormal();

    //pitch angle
    FVector2D xzdir(normalized.X, normalized.Z);
    float pitchRad = signedAngleRadBetween(ZAxis, xzdir) *-1.0f; // test (negieren. Aus. Gründen.)

    //is tested. 
    if(yawConstraint90 && !noYaw){
        float rad90 = M_PI / 2.0f;
    
        //makes yaw angle on the positive side and flips pitch instead!
        if (std::abs(yawRad) > rad90) {
            yawRad += (yawRad > 0.0f) ? -M_PI : M_PI;
            pitchRad *= -1.0f;
        }
        
        
    }

    MMatrix rotationMat;
    rotationMat.yawRadAdd(yawRad);
    rotationMat.pitchRadAdd(pitchRad);

    outYaw = yawRad;
    outPitch = pitchRad;

    return rotationMat;
}

//experimental roll instead of yaw
MMatrix MMatrix::createRotatorFrom(
    FVector &other, //targeted
    FVector2D XAxis,
    FVector2D ZAxis,
    bool yawConstraint90,
    FVector &localFoward
){

    FVector normalized = other.GetSafeNormal();

    float yawRad = 0.0f;
    float pitchRad = 0.0f;
    float rollRad = 0.0f;

    //use roll case
    float dotProductBetweenTargetAndMoveDir = DotProduct2D(localFoward, other);
    if(!useRoll(dotProductBetweenTargetAndMoveDir)){
        //yaw angle
        FVector2D xydir(normalized.X, normalized.Y);
        yawRad = signedAngleRadBetween(XAxis, xydir); //signiert voll umfänglich notwendig anders als pitch
        
        /**
         * wenn sich der zielpunkt nahe der 0 werte befindet,
         * ist der acos(0) = 90 degree, was seltsame
         * sprünge erzeugt.
         */
        bool noYaw = false;
        FVector xyProjection(other.X, other.Y, 0.0f);
        if (false && xyProjection.Size() < 0.1f){ //prevents weird rotations.
            yawRad = 0.0f;
            noYaw = true;
        }

        //idee: yaw angle raus nehmen damit der pitch korrekt projeziert ist! (das ist richtig so!)
        MMatrix removeYaw;
        removeYaw.yawRadAdd(-yawRad);
        normalized = removeYaw * normalized;
        normalized = normalized.GetSafeNormal();
    }else{

        //use roll removal
        FVector2D yzDir(normalized.Y, normalized.Z);
        rollRad = signedAngleRadBetween(ZAxis, yzDir);

        //projection on xz pane to remove roll
        MMatrix removeRoll;
        removeRoll.rollRadAdd(-rollRad);
        normalized = removeRoll * normalized;
        normalized = normalized.GetSafeNormal();

    }


    //pitch angle
    FVector2D xzdir(normalized.X, normalized.Z);
    pitchRad = signedAngleRadBetween(ZAxis, xzdir) *-1.0f; // test (negieren. Aus. Gründen.)

    //is tested. 
    if(yawConstraint90){
        float rad90 = M_PI / 2.0f;
    
        //makes yaw angle on the positive side and flips pitch instead!
        if (std::abs(yawRad) > rad90) {
            yawRad += (yawRad > 0.0f) ? -M_PI : M_PI;
            pitchRad *= -1.0f;
        }
        
        
    }

    MMatrix rotationMat;
    rotationMat.yawRadAdd(yawRad); //(roll, yaw) one only
    rotationMat.yawRadAdd(rollRad); //(roll, yaw) one only
    rotationMat.pitchRadAdd(pitchRad);

    return rotationMat;

}

float MMatrix::DotProduct2D(FVector &a, FVector &b){
    FVector a2D(a.X, a.Y, 0.0f);
    FVector b2D(b.X, b.Y, 0.0f);
    float dot = FVector::DotProduct(a2D.GetSafeNormal(), b2D.GetSafeNormal());

    return dot;
}

bool MMatrix::useRoll(float dotProduct){
    //ab 45 grad any dir. Cos(pi / 4) = 0.707
    return std::abs(dotProduct) <= 0.707f;
}

// ----- EXPIREMENTAL END ------








float MMatrix::unsignedAngleRadBetween(FVector2D &a, FVector2D &b){
    a = a.GetSafeNormal();
    b = b.GetSafeNormal();
    float cosine = a.X * b.X + a.Y * b.Y;
    //cosine = std::clamp(cosine, -1.0f, 1.0f);
    float angle = std::acosf(cosine);
    return angle;
}

float MMatrix::signedAngleRadBetween(FVector2D &a, FVector2D &b){
    //old
    float angle = unsignedAngleRadBetween(a,b) * signForAngle(a, b);
    
    //new
    //float angle = std::atan2(a.X * b.Y - a.Y * b.X, a.X * b.X + a.Y * b.Y);
    
    return angle;
}

/// @brief returns the sign if a and b would be on clockwise order or not, -1 (clockwise angle): 1
/// @param a 
/// @param b 
/// @return 
float MMatrix::signForAngle(FVector2D &a, FVector2D &b){
    /*
    --------ax   bx
    ay   by
    az   bz
    ax   bx
    ay   by
    --------az   bz

    */
    float normalComponentZ = a.X * b.Y - a.Y * b.X;
    if(normalComponentZ < 0.0f){ //clockwise, normale zeigt nach unten
        return -1.0f;
    }
    return 1.0f;
}

/// @brief multiply with another matrix ROTATION ONLY
/// @param other 
void MMatrix::rotate(MMatrix &other){
    
    MMatrix result; 
    for (int col = 0; col < 3; col++){
        for (int row = 0; row < 3; row++){

            float sum = 0.0f;
            for (int i = 0; i < 3; i++){
                float fromThis = get(i, row);
                float fromOther = other.get(col, i);
                sum += fromThis * fromOther;
            }
            result.set(col, row, sum);
        }
    
    }


    // Copy the result back into this matrix
    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            set(col, row, result.get(col, row));
        }
    }




}


/// @brief resets the rotation and rotates in roll pitch yaw order
/// @param other rotation to rotate in
void MMatrix::setRotation(FRotator &other){
    resetRotation();
    if(FMath::IsNaN(other.Roll)){
        other.Roll = 0.0f;
    }
    if(FMath::IsNaN(other.Pitch)){
        other.Pitch = 0.0f;
    }
    if(FMath::IsNaN(other.Yaw)){
        other.Yaw = 0.0f;
    }
    rollRadAdd(MMatrix::degToRadian(other.Roll));
    pitchRadAdd(MMatrix::degToRadian(other.Pitch));
    yawRadAdd(MMatrix::degToRadian(other.Yaw));
}

/// @brief will copy the rotation of the other matrix
/// @param other rotation to copy from matrix
void MMatrix::setRotation(MMatrix &other){
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            set(i, j, other.get(i, j));
        }
    }
}


/// @brief will create a new rotation from a given vector. Translation not touched,
/// rotation is resetted. Only can extract yaw and pitch rotation!
/// @param other 
void MMatrix::setRotation(FVector &other){
    MMatrix rotatorMat = createRotatorFrom(other);
    setRotation(rotatorMat);
}

MMatrix MMatrix::createInverse(){
    // -- using jordan inverse by default --
    return jordanInverse();
}

//3x3 block transponieren für invertieren der rotation (da im orthogonalen koordinaten system)
void MMatrix::invertRotation(){
    //3x3 block transponieren
    swapIndices(4, 1);
    swapIndices(8, 2);
    swapIndices(9, 6);
}




/// @brief requires both indeces to be in bounds!
/// @param a 
/// @param b 
void MMatrix::swapIndices(int a, int b){
    if(a >= 0 && b >= 0 && a < 16 && b < 16){
        float copy = array[a];
        array[a] = array[b];
        array[b] = copy;
    }
}








/// @brief extracts the rotator from this matrix
/// @return FRotator rotation of this matrix
FRotator MMatrix::extractRotator(){
    
    //rotation wird mithilfe von unreal extrahiert 
    //weil ich es selber nicht weiss
    /*
    //so korrekt
    FMatrix MyMatrix(
        FPlane(get(0, 0), get(0, 1), get(0, 2), get(0, 3)), // erste Spalte
        FPlane(get(1, 0), get(1, 1), get(1, 2), get(1, 3)), // zweite Spalte
        FPlane(get(2, 0), get(2, 1), get(2, 2), get(2, 3)), // dritte Spalte
        FPlane(get(3, 0), get(3, 1), get(3, 2), get(3, 3))  // vierte Spalte
    );
    return MyMatrix.Rotator();
    */

    //extracting rotation from a matrix:
    /*

        r11 = a, 
        r12 = b,
        r13 = c

        r21 = e,
        r22 = f, 
        r23 = g

        r31 = i, 
        r32 = j,
        r33 = k


        M = {
            a b c d
            e f g h
            i j k l        
        }

        yaw = atan2(r21, r11)
        pitch = asin(-1 * r31)
        roll = atan2(r32, r33)

        yaw = atan2(b, a)
        pitch = asin(-1 * i)
        roll = atan2(j, k)


    */


    //kontext: get(column, row) umgedreht.
    float _yaw = std::atan2f(get(0, 1), get(0, 0));
    //float _pitch = -1 * std::asinf(get(0, 2));
    float _pitch = -1 * std::asinf(FMath::Clamp(get(0, 2), -1.0f, 1.0f));
    float _roll = std::atan2f(get(1, 2), get(2, 2));

    _yaw = MMatrix::radToDegree(_yaw);
    _pitch = MMatrix::radToDegree(_pitch);
    _roll = MMatrix::radToDegree(_roll);

    /*
    if(FMath::IsNaN(_yaw)){
        _yaw = 0.0f;
    }
    if(FMath::IsNaN(_pitch)){
        _pitch = 0.0f;
    }
    if(FMath::IsNaN(_roll)){
        _roll = 0.0f;
    }*/

    
    //keep rotation like this
    //FRotator Constructor expects FRotator(Yin, Zin, Rin)

    // fix, unreal is weird.
    // IS TESTED, THIS IS CORRECT, DONT PLAY WITH IT!!
    FRotator r(-1 * _pitch, _yaw, -1 * _roll);
    //FRotator r(-1 * _pitch, _yaw, -1 * _roll);
    //FRotator r(-1 * _pitch, _yaw, -1 * _roll);


    return r;

}



MMatrix MMatrix::extarctRotatorMatrix(){
    MMatrix out;
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            out.set(i, j, get(i, j));
        }
    }

    return out;
}

float MMatrix::extractYawAngleRad(){
    FVector forward(1, 0, 0); //x is forward by default
    FVector normalized = *this * forward;
    normalized = normalized.GetSafeNormal();

    FVector2D Xaxis(1.0f, 0); //default axis, x nach vorne schauend
    FVector2D xydir(normalized.X, normalized.Y);
    float yawRad = signedAngleRadBetween(Xaxis, xydir); //signiert voll umfänglich notwendig anders als pitch

    return yawRad;
}

/// @brief converts a position to relative this matrix by using the jordan inverse
/// @param position position to convert, will be adjusted by reference
void MMatrix::transformFromWorldToLocalCoordinates(FVector &position){

	
	//funktoniert
	MMatrix inverted = jordanInverse();
	position = inverted * position;
	
}










/**
 * 
 * new section for inverse
 * 
 */



void MMatrix::set(int column, int row, float value){
    bool lowerRange = column >= 0 && row >= 0;
    bool higherRange = column < 4 && row < 4;
    if(lowerRange && higherRange){
        int index = (row * 4) + column;
        array[index] = value;
    }
}

/// @brief 
/// @param columnX column to get X
/// @param rowY row to get Y
/// @return 
float MMatrix::get(int columnX, int rowY){
    
    bool lowerRange = columnX >= 0 && rowY >= 0;
    bool higherRange = columnX < 4 && rowY < 4;
    if(lowerRange && higherRange){
        int index = (rowY * 4) + columnX;
        return array[index];
    }
    return 0.0f;
}




//was brauchen wir: eine funktion die die determinante einer matrix bestimmt die man 
//am besten als array oder MMatrix übergibt und die obere ecke, und grösse quasi




float MMatrix::det(std::vector<float> &matrix){

    float detCurrent = 0.0f;
    int scale = std::sqrt(matrix.size());
    if(scale == 2){
        return matrix[0] * matrix[3] - matrix[1] * matrix[2]; //det2x2 = ad - bc
    }

    //nur oberste zeile durchgehen und submatrizen determinante holen.
    for (int i = 0; i < scale; i++){
        std::vector<float> submatrix = collectExcept(i, 0, matrix);
        int faktor = i % 2 == 0 ? 1 : -1;
        detCurrent += matrix[i] * faktor * det(submatrix);
    }
    return detCurrent;
}



/// @brief collects the submatrix of a matrix(as 1D vector) and returns it
/// @param x x pos to avoid
/// @param y y pos to avoid
/// @param matrix matrix to filter
/// @return filtered submatrix
std::vector<float> MMatrix::collectExcept(int x, int y, std::vector<float> &matrix){
    std::vector<float> output;
    
    //4x4 = 16
    int scale = std::sqrt(matrix.size());
    int index = 0;
    for(int i = 0; i < scale; i++) //x durchgehen
    {
        for(int j = 0; j < scale; j++){
            if(j != y && i != x){
                output.push_back(matrix[index]);
            }    
            index++;
        }
    }
    return output;
}


/// @brief transposes this matrix as expected
void MMatrix::transpose(){
    /*
    0  1  2  3
    4  5  6  7
    8  9  10 11
    12 13 14 15
    */
    swapIndices(1, 4);
    swapIndices(2, 8);
    swapIndices(6, 9);
    swapIndices(3, 12);
    swapIndices(7, 13);
    swapIndices(11, 14);
}

void MMatrix::transposeRotation(){
    /*
    r0  r1  r2  3
    r4  r5  r6  7
    r8  r9  r10 11
    12 13 14 15
    */
    swapIndices(1, 4);
    swapIndices(2, 8);
    swapIndices(6, 9);
}

//special cases for custom inverse finding
MMatrix MMatrix::transposedRotation(){
    MMatrix copy = *this;
    copy.setTranslation(0, 0, 0);
    copy.transposeRotation();
    return copy;
}

MMatrix MMatrix::invertedTranslation(){
    FVector translation = getTranslation() * -1.0f;
    MMatrix output(translation);
    return output;
}

/// @brief calculates the inverse matrix with jordan gaus algorythm
/// an identity matrix is returned if the inverse is not possible to make (det(A) = 0)
/// the inverse is calculated in O(n^2)
/// @return inverse matrix or identity if an issue occured
MMatrix MMatrix::jordanInverse(){
    MMatrix identity; //operationen auf diese identity matrix auch anwenden,
    MMatrix thisMatrix = *this; //kopie um nicht zu manipulieren
    // das ist am ende die inverse

    for (int x = 0; x < 4; x++){

        /*
        //durch pivot teilen dass 1 in pivot
        float devide = thisMatrix.get(x, x); //sollte das skallierungs element (sX, sY, sZ) = 0 sein
        //ist die matrix natürlich nicht invertierbar!!
        if(devide == 0.0f){
            DebugHelper::logMessage("debug identitydebug MATRIX NICHT INVERTIERBAR");
            MMatrix cleanMatrix;
            return cleanMatrix;
        }*/

        float devide = clampDivisionByZero(thisMatrix.get(x, x));
        thisMatrix.scaleRow(x, (1.0f / devide)); // d * 1 / d = 1, skallieren pivot.
        identity.scaleRow(x, (1.0f / devide));

        //darunter wandern, elemente eliminieren
        for (int y = x + 1; y < 4; y++){

            //gleichung essenziell:
            //a soll 0 sein

            //1 - - - 
            //a - - - 
            //b - - - 
            //c - - -
            // usw

            // a - x * 1 = 0 
            // a = x * 1
            //heisst dann ja arow -= 1row * x und x ist das spalten element darunter
            float lower = thisMatrix.get(x, y);
            thisMatrix.minusForRow(y, x, lower); //von untere, jetzt (1 * something sodass = 0, = something, ist gleich wert.)
            identity.minusForRow(y, x, lower);
        }
    }

    // Eliminate elements above the pivots (new part)
    for (int x = 3; x >= 0; x--) {
        for (int y = x - 1; y >= 0; y--) {
            float upper = thisMatrix.get(x, y);
            thisMatrix.minusForRow(y, x, upper); // Eliminate element in (y, x)
            identity.minusForRow(y, x, upper); // Do the same for identity matrix
        }
    }
    

    /*
    FString message = FString::Printf(TEXT("debug inverse:"));
	message += identity.asString();
    DebugHelper::logMessage(message);
    */

    return identity;
}

/// @brief a very dangerous fix to let the matrix be invertable. Does introduce numeric
/// inaccuracy
/// THIS VALUES ARE TESTED FOR THE BONECONTROLLER AND SEEMS TO WORK OK! DO NOT CHANGE!
/// @param other value to check
/// @return value to allow invertion of the matrix with jordan gauß verfahren
float MMatrix::clampDivisionByZero(float other){
    if(std::abs(other) <= 0.000001f){
        return 0.00001f;
    }
    return other;
}


/// @brief scales a row with a factor 
/// @param row row to scale
/// @param scale faktor
void MMatrix::scaleRow(int row, float scale){
    /*int start = row * 4;
    int end = (row + 1) * 4;
    for (int i = start; i < end; i++){
        array[i] *= scale;
    }*/

    for (int i = 0; i < 4; i++){
        float val = get(i, row) * scale;
        set(i, row, val);
    }
}

/// @brief subtrahiere von row , other row, mal einem faktor
/// @param row row to apply subtraction
/// @param otherRow subtract this row
/// @param faktor faktor of otherRow 
void MMatrix::minusForRow(int row, int otherRow, float faktor) {

    for (int i = 0; i < 4; i++){
        float otherValueScaled = get(i, otherRow) * faktor;
        float thisValue = get(i, row) - otherValueScaled;
        set(i, row, thisValue);
    }

    /*
    int startRow = row * 4;
    int startOtherRow = otherRow * 4;
    for (int i = 0; i < 4; i++) {
        array[startRow + i] -= faktor * array[startOtherRow + i];
    }*/
}






FVector MMatrix::lookDirXForward(){
    FVector look(1.0f, 0.0f, 0.0f);
    MMatrix a = extarctRotatorMatrix();
    return a * look;
}




void MMatrix::scaleUniform(float value){
    scale(value, value, value);
}

void MMatrix::scale(float x, float y, float z){
    MMatrix scaleMat;
    scaleMat.set(0, 0, x);
    scaleMat.set(1, 1, y);
    scaleMat.set(2, 2, z);
    *this *= scaleMat;
}

/**
 * static helpers
 */
void MMatrix::rotateVectorDeg2D(float angleDeg, FVector2D &vector){
    MMatrix::rotateVectorRad2D(MMatrix::degToRadian(angleDeg), vector);
}

void MMatrix::rotateVectorRad2D(float angleRad, FVector2D &vector){
    /*          
               x
               y
    cos -sin |  
    sin cos  | 
    */
    float cos = std::cos(angleRad);
    float sin = std::sin(angleRad);

    float x = (cos * vector.X) + (-1 * sin * vector.Y);
    float y = (sin * vector.X) + (cos * vector.Y);

    vector.X = x;
    vector.Y = y;
}







//Matrix3x3 support
void MMatrix::setRotation(std::vector<float> &values){
    if(values.size() == 9){

        /*
        0  1  2  -
        4  5  6  -
        8  9 10  -
        -  -  -  -
        */
        array[0] = values[0];
        array[1] = values[1];
        array[2] = values[2];
        array[4] = values[3];
        array[5] = values[4];
        array[6] = values[5];
        array[8] = values[6];
        array[9] = values[7];
        array[10] = values[8];

    }
}



std::vector<float> MMatrix::CopyRotation(){
    std::vector<float> values = {
        array[0],
        array[1],
        array[2],
        array[4],
        array[5],
        array[6],
        array[8],
        array[9],
        array[10]};
    return values;
}
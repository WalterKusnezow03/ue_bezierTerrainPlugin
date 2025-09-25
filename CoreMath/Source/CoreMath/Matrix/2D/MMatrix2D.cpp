// Fill out your copyright notice in the Description page of Project Settings.

#include "MMatrix2D.h"

#include "CoreMinimal.h"
#include <cmath>
#include "Math/Rotator.h"


MMatrix2D::MMatrix2D()
{
    makeIdentity();
}

MMatrix2D::~MMatrix2D()
{

}

MMatrix2D::MMatrix2D(FVector2D pos){
    makeIdentity();
    SetTranslation(pos);
}

void MMatrix2D::SetTranslation(FVector2D vec){
    /*
    1 0 tx
    0 1 ty
    0 0 1
    */
    array[2] = vec.X;
    array[5] = vec.Y;
}
FVector2D MMatrix2D::getTranslation() const {
    return FVector2D(array[2], array[5]);
}

bool MMatrix2D::IsZeroScaleMatrix() const {
    float epsilon = 0.000001f;
    return std::abs(array[0]) < epsilon &&
           std::abs(array[1]) < epsilon &&
           std::abs(array[3]) < epsilon &&
           std::abs(array[4]) < epsilon;
}

void MMatrix2D::makeIdentity(){
    makeZero();
    array[0] = 1.0f;
    array[4] = 1.0f;
    array[8] = 1.0f;
}

void MMatrix2D::makeZero(){
    for (int i = 0; i < size; i++){
        array[i] = 0.0f;
    }
}


/// @brief copy constructor
/// @param other 
MMatrix2D::MMatrix2D(const MMatrix2D &other){
    makeIdentity();
    if (&other != this)
    {
        *this = other;
    }
}

//copy constructor
MMatrix2D& MMatrix2D::operator=(const MMatrix2D &other){
    if(this == &other){
        return *this;
    }

    //copy data
    for (int i = 0; i < 9; i++){
        array[i] = other.array[i];
    }

    return *this;
}


/// @brief multiply with another matrix
/// @param other 
void MMatrix2D::operator*=(const MMatrix2D &other){
    MMatrix2D r = *this * other;
    *this = r;
}



/// @brief multiply and return result
/// @param other other matrix to append like: This * other 
/// @return returns a new matrix
MMatrix2D MMatrix2D::operator*(const MMatrix2D &other) const{
    
    MMatrix2D result; // Temporary matrix to store the result

    for (int row = 0; row < 3; row++) { // Iterate through the rows of 'this' matrix
        for (int col = 0; col < 3; col++) { // Iterate through the columns of 'other' matrix
            float sum = 0.0f;
            for (int inner = 0; inner < 3; inner++) { // Perform dot product
                sum += array[row * 3 + inner] * other.array[inner * 3 + col];
            }
            result.array[row * 3 + col] = sum; // Store the result in the temporary matrix
        }
    }
    
    return result;
}





/// @brief multiply with a vector 
/// @param other 
/// @return 
FVector2D MMatrix2D::operator*(const FVector2D &other) const{
    FVector2D resultVec;

    float row[] = {other.X, other.Y, 1.0f}; //1.0 for homogen
    float resultRow[] = {0.0f, 0.0f, 0.0f};
    
    for (int thisRow = 0; thisRow < 3; thisRow++) //jede row dieser matrix
    {
        float result = 0.0f;
        for (int c = 0; c < 3; c++){
            result += array[thisRow * 3 + c] * row[c];
        }
        resultRow[thisRow] = result;
    }
    

    resultVec.X = resultRow[0];
    resultVec.Y = resultRow[1];
    return resultVec;
}

FVector2f MMatrix2D::operator*(const FVector2f &other) const{
    FVector2f resultVec;

    float row[] = {other.X, other.Y, 1.0f}; //1.0 for homogen
    float resultRow[] = {0.0f, 0.0f, 0.0f};
    
    for (int thisRow = 0; thisRow < 3; thisRow++) //jede row dieser matrix
    {
        float result = 0.0f;
        for (int c = 0; c < 3; c++){
            result += array[thisRow * 3 + c] * row[c];
        }
        resultRow[thisRow] = result;
    }
    

    resultVec.X = resultRow[0];
    resultVec.Y = resultRow[1];
    return resultVec;
}



/// @brief util converts degrees to radian
/// @param deg deg input
/// @return 
float MMatrix2D::degToRadian(float deg){
    float angleInRadians = ((deg * M_PI) / 180.0); // Umwandlung in Bogenmaß
    return angleInRadians;
}

float MMatrix2D::radToDegree(float rad){
    float angleInDeg = ((rad * 180.0) / M_PI); // Umwandlung in Bogenmaß
    return angleInDeg;
}


/// --- Scale ---
void MMatrix2D::Scale(float scale){
    Scale(scale, scale);
}
void MMatrix2D::Scale(float scaleX, float scaleY){
    MMatrix2D scaleMat = MakeScaleMatrix(scaleX, scaleY);
    *this *= scaleMat;
}


MMatrix2D MMatrix2D::MakeScaleMatrixUniform(float scale){
    return MakeScaleMatrix(scale, scale);
}

MMatrix2D MMatrix2D::MakeScaleMatrix(float scaleX, float scaleY){
    MMatrix2D scaleMat;
    /*
    sx 0 0
    0 sy 0
    0  0 1
    */
    scaleMat.array[0] = scaleX;
    scaleMat.array[4] = scaleY;
    return scaleMat;
}

/// --- Rotation ---

void MMatrix2D::SetRotation(const FVector2D &other){
    FVector2D xAxis(1.0f, 0.0f);
    SetRotation(xAxis, other);
}

void MMatrix2D::SetRotation(const FVector2D &_axis, const FVector2D &_other){

    FVector2D axis = _axis.GetSafeNormal();
    FVector2D other = _other.GetSafeNormal();

    float dot = FVector2D::DotProduct(axis, other);
    float nZ = axis.X * other.Y - axis.Y * other.X; // 2D-"Kreuzprodukt" (entspricht Z-Komponente)
    float angle = std::atan2(nZ, dot);

    RadAdd(angle);
}


void MMatrix2D::RadAdd(float a){
    if(a == 0.0f){
        return;
    }

    /*
    0 1
    3 4
    cos, -sin
    sin, cos
    */
    MMatrix2D rotMatrix;
    rotMatrix.makeIdentity();
    float cos = std::cos(a);
    float sin = std::sin(a);

    rotMatrix.array[0] = cos;
    rotMatrix.array[1] = sin * -1;
    rotMatrix.array[3] = sin;
    rotMatrix.array[4] = cos;

    rotate(rotMatrix);
}


/// @brief multiply with another matrix ROTATION ONLY
/// @param other 
void MMatrix2D::rotate(MMatrix2D &other){
    
    MMatrix2D result; 
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



/// @brief requires both indeces to be in bounds!
/// @param a 
/// @param b 
void MMatrix2D::swapIndices(int a, int b){
    if(a >= 0 && b >= 0 && a < size && b < size){
        float copy = array[a];
        array[a] = array[b];
        array[b] = copy;
    }
}








/// @brief extracts the rotator from this matrix
/// @return FRotator rotation of this matrix
float MMatrix2D::extractAngle(){
    
    //extracting rotation from a 4x4 matrix:
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


    //kontext: get(column, row)
    float _yaw = std::atan2f(get(0, 1), get(0, 0));
    return _yaw;
}



/**
 * 
 * new section for inverse
 * 
 */



void MMatrix2D::set(int column, int row, float value){
    bool lowerRange = column >= 0 && row >= 0;
    bool higherRange = column < 3 && row < 3;
    if(lowerRange && higherRange){
        int index = (row * 3) + column;
        array[index] = value;
    }
}

/// @brief 
/// @param columnX column to get X
/// @param rowY row to get Y
/// @return 
float MMatrix2D::get(int columnX, int rowY){
    
    bool lowerRange = columnX >= 0 && rowY >= 0;
    bool higherRange = columnX < 3 && rowY < 3;
    if(lowerRange && higherRange){
        int index = (rowY * 3) + columnX;
        return array[index];
    }
    return 0.0f;
}

/// @brief transposes this matrix as expected
void MMatrix2D::transpose(){
    /*
    0  1  2  
    3  4  5
    6  7  8
    */
    swapIndices(1, 3);
    swapIndices(2, 6);
    swapIndices(5, 7);
}

void MMatrix2D::transposeRotationBlock(){
    /*
    R t
    0 1
    */
    
    /*
    0  1  2  
    3  4  5
    6  7  8
    */
   swapIndices(1, 3);
}





FString MMatrix2D::asString(){
    FString out;
    for (int i = 0; i < 3; i++)
    {
        FString s = FString::Printf(
            TEXT("[%.2f, %.2f, %.2f]"),
            get(i, 0),
            get(i, 1),
            get(i, 2)
        );
        out += s;
    }
    return out;
}



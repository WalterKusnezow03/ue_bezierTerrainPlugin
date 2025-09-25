#include "FBoundingBox2D.h"


FBoundingBox2D::FBoundingBox2D(){
    Reset();
}
FBoundingBox2D::~FBoundingBox2D(){}

void FBoundingBox2D::Reset(){
    topLeft = FVector2D(0, 0);
    bottomRight = FVector2D(0, 0);
}

FBoundingBox2D::FBoundingBox2D(const TArray<FVector2D> &buffer){
    Reset();
    Update(buffer);
}

FBoundingBox2D::FBoundingBox2D(const FBoundingBox2D & other){
    if(this != &other){
        *this = other;
    }
}

FBoundingBox2D &FBoundingBox2D::operator=(const FBoundingBox2D & other){
    if(this != &other){
        topLeft = other.topLeft;
        bottomRight = other.bottomRight;
    }
    return *this;
}

void FBoundingBox2D::Update(const TArray<FVector2D> &buffer){
    for (int i = 0; i < buffer.Num(); i++){
        Update(buffer[i]);
    }
}

void FBoundingBox2D::Update(const FVector2D &v0, const FVector2D &v1, const FVector2D &v2){
    Update(v0);
    Update(v1);
    Update(v2);
}

void FBoundingBox2D::Update(const FVector2D &pos){
    topLeft.X = std::min(topLeft.X, pos.X);
    topLeft.Y = std::min(topLeft.Y, pos.Y);

    bottomRight.X = std::max(bottomRight.X, pos.X);
    bottomRight.Y = std::max(bottomRight.Y, pos.Y);
}

FVector2D FBoundingBox2D::min()const{
    return topLeft;
}

FVector2D FBoundingBox2D::max()const{
    return bottomRight;
}

FVector2D FBoundingBox2D::minXmaxY() const {
    return FVector2D(
        min().X,
        max().Y
    );
}

FVector2D FBoundingBox2D::maxXminY() const {
    return FVector2D(
        max().X,
        min().Y
    );
}

FVector2D FBoundingBox2D::center() const{
    return (min() + max() / 2.0f);
}

FVector2D FBoundingBox2D::size() const {
    return FVector2D(sizeX(), sizeY());
}

float FBoundingBox2D::sizeX()const{
    //AB = B - A
    return std::abs(bottomRight.X - topLeft.X);
}

float FBoundingBox2D::sizeY()const{
    //AB = B - A
    return std::abs(bottomRight.Y - topLeft.Y);
}

bool FBoundingBox2D::OtherIsInside(const FBoundingBox2D &other){
    bool allFlags =
        min().X <= other.min().X &&
        min().Y <= other.min().Y &&
        max().X >= other.max().X &&
        max().Y >= other.max().Y;
    return allFlags;
}


FString FBoundingBox2D::ToString() const {
    FString result = TEXT("Bounds:");
    result += topLeft.ToString();
    result += bottomRight.ToString();
    return result;
}


TArray<FVector2D> FBoundingBox2D::asVertecies(){
    /*
    /// @brief min coordinate on x and y
    FVector2D topLeft;
    /// @brief max coordiate on x and y
    FVector2D bottomRight;
    */
    TArray<FVector2D> output = {
        topLeft,
        FVector2D(topLeft.X, bottomRight.Y),
        bottomRight,
        FVector2D(bottomRight.X, topLeft.Y),
    };
    return output;
}
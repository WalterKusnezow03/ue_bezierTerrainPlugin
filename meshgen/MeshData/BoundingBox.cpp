#include "BoundingBox.h"


BoundingBox::BoundingBox(){
    bottomLeftNear = FVector(0, 0, 0);
    topRightLeft = FVector(0, 0, 0);
}

BoundingBox::BoundingBox(TArray<FVector> &vertecies){
    bottomLeftNear = FVector(0, 0, 0);
    topRightLeft = FVector(0, 0, 0);
    updateBoundsIfNeeded(vertecies);
}

BoundingBox::~BoundingBox(){

}

void BoundingBox::updateBoundsIfNeeded(TArray<FVector> &vertecies){
    for (int i = 0; i < vertecies.Num(); i++){
        updateBoundsIfNeeded(vertecies[i]);
    }
}

void BoundingBox::updateBoundsIfNeeded(FVector &other){
    bottomLeftNear.X = std::min(bottomLeftNear.X, other.X);
    bottomLeftNear.Y = std::min(bottomLeftNear.Y, other.Y);
    bottomLeftNear.Z = std::min(bottomLeftNear.Z, other.Z);

    topRightLeft.X = std::max(topRightLeft.X, other.X);
    topRightLeft.Y = std::max(topRightLeft.Y, other.Y);
    topRightLeft.Z = std::max(topRightLeft.Z, other.Z);
}

bool BoundingBox::isInsideBoundingbox(FVector &other){
    return other.X > bottomLeftNear.X &&
           other.Y > bottomLeftNear.Y &&
           other.Z > bottomLeftNear.Z &&
           other.X < topRightLeft.X &&
           other.Y < topRightLeft.Y &&
           other.Z < topRightLeft.Z;
}



FVector BoundingBox::bottomLeftNearVertex(){
    return bottomLeftNear;
}

FVector BoundingBox::topRightLeftVertex(){
    return topRightLeft;
}
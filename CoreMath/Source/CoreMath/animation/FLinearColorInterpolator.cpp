#include "FLinearColorInterpolator.h"


FLinearColorInterpolator::FLinearColorInterpolator(){

}
FLinearColorInterpolator::~FLinearColorInterpolator(){

}

FLinearColor FLinearColorInterpolator::interpolation(
    FLinearColor &fromIn, 
    FLinearColor &toIn, 
    float skalarCurrent
){
    FLinearColor direction = toIn - fromIn; //AB = B -A
    //gx = A + r (B - A)
    return fromIn + skalarCurrent * direction;
}

float FLinearColorInterpolator::Distance(FLinearColor &a, FLinearColor &b){
    return FVector::Dist(to3D(a), to3D(b));
}

FVector FLinearColorInterpolator::to3D(FLinearColor &color){
    return FVector(color.R, color.G, color.B);
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "ripple.h"

ripple::ripple(FVector &impactPoint)
{
    init(impactPoint);
}

ripple::~ripple()
{
}

void ripple::init(FVector &impactPoint){
    impact = impactPoint;
    time = 0.0f;
    radius = 0.0f;
}

void ripple::increaseRadius(float deltaTime){
    time += deltaTime;
    radius += velocity * deltaTime; // x(t) = x0 + v0t + 1/2at^2
}

bool ripple::timeExceeded(){
    return maxlifeTime < time;
}


void ripple::changeHeightBasedOnDistance(FVector &vertex){
    float toCenter = FVector::Dist(vertex, impact);
    if(toCenter > 0.01f){

        bool inside = toCenter < radius;

        //float frac = toCenter / radius; // 2 / 2 = 1, 100%; 5 / 2 = 2,5
        float frac = radius / toCenter; // 2 / 2 = 1, 100%; 2 / 5 = 0.4, 40%
        float heightAdd = 100.0f * frac;
        if(inside){
            heightAdd *= -1; //remove added offset again
        }

        vertex.Z += heightAdd;
    }
}
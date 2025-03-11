// Fill out your copyright notice in the Description page of Project Settings.


#include "ripple.h"
#include <limits>

ripple::ripple(FVector &impactPoint)
{
    init(impactPoint, std::numeric_limits<float>::max());
}

ripple::ripple(FVector &impactPoint, float newMaxRadius)
{
    init(impactPoint, newMaxRadius);
}

ripple::ripple(const ripple &other){
    *this = other;
}

ripple &ripple::operator=(const ripple &other){
    if(this == &other){
        return *this;
    }
    maxlifeTime = other.maxlifeTime;
    velocity = other.velocity;
    time = other.time;
    radius = other.radius;
    impact = other.impact;
    waveHeight = other.waveHeight;

    return *this;
}

ripple::~ripple()
{
}

void ripple::init(FVector &impactPoint, float maxRadiusIn){
    setMaxRadius(maxRadiusIn);
    impact = impactPoint;
    time = 0.0f;
    radius = 0.0f;

    maxlifeTime = 10.0f;
}

///@brief changes the max radius which is with time the factor for the ripple to dissappear
void ripple::setMaxRadius(float radiusIn){
    maxRadius = std::abs(radiusIn);
}

void ripple::Tick(float deltaTime){
    time += deltaTime;
    radius += velocity * deltaTime; // x(t) = x0 + v0t + 1/2at^2
}

bool ripple::timeExceeded(){
    return (maxlifeTime < time) || (radius > maxRadius);
}

/// @brief returns the wave height based on time (distance from initial center indirectly)
/// @return wave height 
float ripple::waveHeightBasedOnTime(){
    float timeScalar = (maxlifeTime - time) / maxlifeTime; // distTarget / distAll
    return timeScalar * waveHeight;
}

///@brief changes the vertex height based on distance to vertex+actor (combined)
void ripple::changeHeightBasedOnDistance(FVector &vertex, FVector &offsetActor){
    float toCenter = FVector::Dist(vertex + offsetActor, impact);
    if(toCenter > 0.01f){

        
        float distFromRadius = std::abs(toCenter - radius);
        if(distFromRadius < 1.0f){
            distFromRadius = 1.0f;
        }

        if (distFromRadius < rippleWidth)//range 50cm z.b.
        {
            float frac = 1.0f * cos(distFromRadius / rippleWidth);
            float heightAdd = waveHeightBasedOnTime() * frac;
            vertex.Z += heightAdd;
        }

    }
}



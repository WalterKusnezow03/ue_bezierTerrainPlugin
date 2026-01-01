// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyFrame.h"

KeyFrame::KeyFrame(FVector positionIn, float timeToFrameIn)
{
    position = positionIn;
    setTimeToFrame(timeToFrameIn);
    grounded = false;
}

KeyFrame::KeyFrame(FVector positionIn, float timeToFrameIn, bool groundedIn){
    position = positionIn;
    setTimeToFrame(timeToFrameIn);
    grounded = groundedIn;
}

KeyFrame::~KeyFrame()
{
}

void KeyFrame::setTimeToFrame(float timeToFrameIn){
    timeToFrame = std::abs(timeToFrameIn);
}

FVector KeyFrame::readposition(){
    return position;
}

float KeyFrame::readTimeToFrame(){
    return timeToFrame;
}

bool KeyFrame::mustBeGrounded(){
    return grounded;
}


float KeyFrame::distanceTo(KeyFrame &other){
    return FVector::Dist(other.position, position);
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "timer.h"



timer::timer(){

}

/// @brief starts the timer at a given time
/// @param time time will be absolute
/// @param resetsItselfIn tells if the timer resets itsself when the timesUpFunction returns true
timer::timer(float time, bool resetsItselfIn)
{
    Begin(time, resetsItself);
}

timer::~timer()
{
}


/// @brief starts the timer at a given time
/// @param time time will be absolute
/// @param resetsItselfIn tells if the timer resets itsself when the timesUpFunction returns true
void timer::Begin(float time, bool resetsItselfIn){
    time = std::abs(time);
    initialTime = time;
    timeLeft = time;
    resetsItself = resetsItselfIn;
}

/// @brief will begin the timer, one time timer!
/// @param time 
void timer::Begin(float time){
    Begin(time, false);
}

void timer::Tick(float deltaTime){
    timeLeft -= deltaTime;
}

bool timer::TickWithTimesUpReset(float deltatime){
    Tick(deltatime);
    return timesUp();
}

bool timer::timesUp()
{
    bool timeIsUp = timeLeft < 0.05f;
    if(timeIsUp && resetsItself){
        timeLeft = initialTime;
    }
    return timeIsUp;
}

float timer::scalar(){
    float reached = initialTime - timeLeft;
    float denominator = std::max(initialTime, 0.1f);
    return reached / denominator;
}
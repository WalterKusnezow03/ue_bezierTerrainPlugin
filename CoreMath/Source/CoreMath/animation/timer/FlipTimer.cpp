#include "FlipTimer.h"

FlipTimer::FlipTimer() : timer() {

};

FlipTimer::~FlipTimer(){
    
}

bool FlipTimer::timesUp(){
    if(timer::timesUp()){
        bFlipped = !bFlipped;
        return true;
    }
    return false;
}

float FlipTimer::scalar(){
    float scalarRaw = timer::scalar();
    if(bFlipped){
        scalarRaw = 1.0f - scalarRaw;
    }
    return scalarRaw;
}
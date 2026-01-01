#include "PayloadTimer.h"

PayloadTimer::PayloadTimer() : timer() {

}
PayloadTimer::~PayloadTimer(){

}


bool PayloadTimer::TickWithTimesUpReset(float deltatime){
    if(timer::TickWithTimesUpReset(deltatime)){
        NotifyAll();
        return true;
    }
    return false;
}

void PayloadTimer::AddPayload(Payload &load){
    executeOnFinish.Add(load);
}

void PayloadTimer::NotifyAll(){
    for (int i = 0; i < executeOnFinish.Num(); i++){
        Payload &current = executeOnFinish[i];
        current.Execute();
    }
    executeOnFinish.Empty();
}
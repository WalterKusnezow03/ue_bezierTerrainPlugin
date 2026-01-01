#pragma once

#include "CoreMinimal.h"

#include "timer.h"


class COREMATH_API FlipTimer : public timer{

public:
    FlipTimer();
    virtual ~FlipTimer();

    virtual bool timesUp();
    virtual float scalar();

private:
    bool bFlipped = false; //marks state
};
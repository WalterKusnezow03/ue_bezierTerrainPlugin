// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoreMath/animation/timer/PayloadInterface/IPayloadInterface.h"

#include "CoreMath/animation/timer/PayloadInterface/Payload.h"

/**
 * 
 */
class COREMATH_API PayloadTimer : public timer
{
public:
	PayloadTimer();
	virtual ~PayloadTimer();

	virtual bool TickWithTimesUpReset(float deltaTime) override;
    void AddPayload(Payload &load);

    void NotifyAll();

private:
    TArray<Payload> executeOnFinish;
};

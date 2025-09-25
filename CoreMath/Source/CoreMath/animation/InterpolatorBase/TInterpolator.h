#pragma once

#include "CoreMinimal.h"


/// @brief template base for any Vector like Interpolator. Is very abstract, emerged from FVector interpolation
/// but abstraction is needed for more types.
/// @tparam T 
template <typename T>
class COREMATH_API TInterpolator {
public:
    TInterpolator(){}
    virtual ~TInterpolator(){}

    void setTarget(T &fromIn, T &toTarget, float timeToFrameIn){
        overrideStart(fromIn);
        overrideTarget(toTarget);
        overrideTime(timeToFrameIn);
        resetReachedflag();
        targetSetup = true;
        updateReachedFlagBasedOnDistance();
    }

    void overrideStart(T &fromIn){
        from = fromIn;
    }

    void overrideTarget(T &toIn){
        target = toIn;
    }

    virtual T interpolate(float DeltaTime){
        if(reached){
            return target;
        }
        deltaTime += DeltaTime;
        float skalarCurrent = skalar();
        UpdateReachedFlag(skalarCurrent);
        T gx = interpolation(from, target, skalarCurrent);
        return gx;
    }

    float TimeToFrame(){
        return timeToFrameSaved;
    }

    ///@brief will return whether the target T is reached, otherwise true.
    ///if no target setup, always true.
    bool hasReachedTarget(){
        if(targetSetup){
            return reached;
        }
        return true;
    }

    virtual bool endReached(){
        return reached;
    }

    void overrideTime(float timeIn){
        timeToFrameSaved = std::abs(timeIn);
        deltaTime = 0.0f;
        resetReachedflag();
    }

    void resetReachedflag(){
        reached = false;
    }

    virtual T interpolation(T &fromIn, T &toIn, float skalarCurrent) = 0;
    /*
    virtual T interpolation(T &fromIn, T &toIn, float skalarCurrent){
        T connect = toIn - fromIn; // AB = B - A
        //gx = A + r (B - A)
        T interpolated = fromIn + skalarCurrent * connect;
        return interpolated;
    }
    */

    // ----- SPEED RELATIVE OVERRIDE ------


    // -- Very important for Speed relative Override of start --

    /// MUST BE OVERRIDEN
    /// needed for dynamic speed override, same speed while changing the
    /// start trajectory
    virtual float Distance(T &a, T &b) = 0;

    //if a distance is short, mark reached true
    void updateReachedFlagBasedOnDistance(){
        float epsilon = 0.1f; //0.5f
        reached = Distance(from, target) <= epsilon;
    }


    void overrideStartSpeedRelative(T &newStart){
        /**
         * was passiert hier:
         * alte distanz und time to frame ergeben velocity
         *
         * velocity: m/s
         * newDist: m
         * 
         * x is Time searched at some velocity
         * newM / x = velocity
         * newM = velocity * x
         * x = newM / velocity
         * 
         *
         * newtime = (m) / (m/s) = m * (s/m) = s
         * 
         * metersnew / speed = m * (s/m) = s
         * 
         */

        float distanceOld = Distance(from, target);
        float speed = distanceOld / timeToFrameSaved; //sei distanz 40m und ttf 2s, dann sinds 20ms

        // Aktualisiere Startpunkt
        overrideStart(newStart);

        // Neue Time-to-Frame berechnen
        float newDistance = Distance(from, target);
        float newTimeToFrame = newDistance / speed;

        // Neue deltaTime basierend auf dem alten Fortschritt
        timeToFrameSaved = newTimeToFrame;

        if(timeToFrameSaved < 0.01f){
            reached = true;
        }
        if(!reached){
            updateReachedFlagBasedOnDistance();
        }
    }

    //experimental.
    void overrideEndSpeedRelative(T &newEnd){
        /**
         * was passiert hier:
         * alte distanz und time to frame ergeben velocity
         *
         * velocity: m/s
         * newDist: m
         *
         * newtime = (m) / (m/s) = m * (s/m) = s
         * 
         * metersnew / speed = m * (s/m) = s
         * 
         */

        float distanceOld = Distance(from, target);
        float speed = distanceOld / timeToFrameSaved; //sei distanz 40m und ttf 2s, dann sinds 20ms

        overrideTarget(newEnd);

        // Neue Time-to-Frame berechnen
        float newDistance = Distance(from, target);
        float newTimeToFrame = newDistance / speed;

        // Neue deltaTime basierend auf dem alten Fortschritt
        timeToFrameSaved = newTimeToFrame;

        if(timeToFrameSaved < 0.01f){
            reached = true;
        }
        if(!reached){
            updateReachedFlagBasedOnDistance();
        }
    }








    bool TargetSetupFlag(){
        return targetSetup;
    }

    void ResetTargetSetupFlag(){
        targetSetup = false;
    }

    void resetDeltaTime(){
        deltaTime = 0.0f;
        reached = false;
    }

    T readFrom(){
        return from;
    }

    T readTarget(){
        return target;
    }

protected:
    float skalar(){
        //hier muss ein epsilon value sein!
        //1 / 0 = unendlich
        float epsilon = 0.1f;
        if (timeToFrameSaved <= 0.0f + epsilon)
        {
            return 1.0f;
        }

        float skal = deltaTime / timeToFrameSaved; //fractionThis = this / all
        
        if(skal > 1.0f){
            skal = 1.0f;
        }
        if(skal < 0.0f){
            skal = 0.0f;
        }

        // Berechnet den Skalierungsfaktor `t`, der zwischen 0 und 1 liegt
        return skal; //t / 1 quasi.

    }


    void UpdateReachedFlag(float skalarCurrent){
        if(deltaTime >= timeToFrameSaved){
            reached = true;
        }
        if(skalarCurrent >= 1.0f){
            reached = true;
        }
    }



protected:
    T from;
    T target;

    bool reached = false;
    float timeToFrameSaved = 0.0f;
    float deltaTime = 0.0f;

    bool targetSetup = false;
};
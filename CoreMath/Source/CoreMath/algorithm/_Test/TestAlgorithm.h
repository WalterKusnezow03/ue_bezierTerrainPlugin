#pragma once

#include "CoreMinimal.h"


class COREMATH_API TestAlgorithm {

public:
    static void Test();

private:
    static void TestInnerHullFinder();
    static void TestPolygonHit();

    static void TestRasterizer();
    static void TestRasterizerFitting();


    template <typename T>
    static FString makeString(TArray<T> &array){
        FString output;
        for (int i = 0; i < array.Num(); i++){
            FString asString = array[i].ToString(); //will be supported for FVector
            output += FString::Printf(TEXT("(%s)"), *asString);
        }
        return output;
    }

    static void LogMessage(FString s);
    static void LogMessage(FString prefix, FString s);
    static void LogMessage(FString prefix, bool testSucess);

private:
    TestAlgorithm() {};
    ~TestAlgorithm() {};


};
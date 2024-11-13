// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class P2_API DebugHelper
{
public:
	DebugHelper();
	~DebugHelper();

	//debugging
	static void Debugtest(UWorld *world);

	//Messages
	static void showScreenMessage(FString s);
	static void showScreenMessage(FString s, FColor color);
	static void showScreenMessage(FString s, int argument);
	static void showScreenMessage(FString s, float argument);
	static void showScreenMessage(FString s, int argument, FColor color);
	static void showScreenMessage(FString s, FVector a, FVector b, FColor color);
	static void showScreenMessage(FString s, FVector2D a, FVector2D b, FColor color);
	static void showScreenMessage(FVector vec);

	//Log Messages
	static void logMessage(FString s);
	static void logMessage(FString s, FVector a, FVector b);
	static void logMessage(FString s, FVector a);
	static FString FVectorToString(FVector vec);

	//time
	static double timeSum;
	static void addTime(double time);
	static void logTime(FString message);

	//FVector
	static void showLineBetween(UWorld *world, FVector Start, FVector End);
	static void showLineBetween(UWorld *world, FVector Start, FVector End, FColor color);
	static void showLineBetween(UWorld *worldin, FVector Start, FVector End, FColor color, float time);
	static void showLine(UWorld *world, std::vector<FVector> &vec);
	static void showLine(UWorld *world, std::vector<FVector> &vec, FColor color);
	static void showLine(UWorld *world, std::vector<FVector> &vec, FColor color, int scale);
	static void showLine(UWorld *world, std::vector<FVector> &vec, FColor color, float time);
	static void showLine(UWorld *world, TArray<FVector> &vec);
	static void showLine(UWorld *world, TArray<FVector> &vec, FColor color);
	static void showLine(UWorld *world, TVector<FVector> &vec, FColor color);


	//Fvector 2D
	static void showLineBetween(UWorld *world, FVector2D Start, FVector2D End);
	static void showLineBetween(UWorld *world, FVector2D Start, FVector2D End, int zOffset);
	static void showLineBetween(UWorld *world, FVector2D Start, FVector2D End, int zOffset, FColor color);

private:
	static FString connect(int a, int b);

};

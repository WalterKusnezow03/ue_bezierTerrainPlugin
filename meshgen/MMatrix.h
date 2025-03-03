// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class P2_API MMatrix
{
public:
	MMatrix();
	~MMatrix();

	MMatrix(FVector &other);
	MMatrix(FRotator &other);
	MMatrix(const MMatrix &other);
	MMatrix& operator=(const MMatrix &other);

	void setTranslation(MMatrix &other);
	void setTranslation(FVector &pos);
	void setTranslation(float x, float y, float z);
	FVector getTranslation();

	void operator+=(FVector &other);
	void operator-=(FVector &other);

	void operator+=(MMatrix &other);
	void operator*=(MMatrix & other);

	FVector operator*(FVector &other);
	MMatrix operator*(MMatrix &other);

	FString asString();

	void pitchRad(float angle);
	void rollRad(float angle);
	void yawRad(float angle);

	void rollRadAdd(float angle);
	void pitchRadAdd(float angle);
	void yawRadAdd(float angle);

	void scaleUniform(float value);
	void scale(float x, float y, float z);

	static float degToRadian(float deg);
	static float radToDegree(float rad);

	static MMatrix createRotatorFromRad(float x, float y, float z);
	static MMatrix createRotatorFromDeg(float x, float y, float z);
	static MMatrix createRotatorFrom(FRotator &other);
	static MMatrix createRotatorFrom(FVector &other);
	static MMatrix createRotatorFrom(FVector &other, FVector2D XAxis, FVector2D ZAxis);



	void resetRotation();

	void rotate(MMatrix &other);
	void setRotation(FRotator &other);
	void setRotation(MMatrix &other);
	void setRotation(FVector &other);

	MMatrix createInverse();

	FRotator extractRotator();
	MMatrix extarctRotatorMatrix();

	void transformFromWorldToLocalCoordinates(FVector &position);

	void invertRotation();

	static void rotateVectorDeg2D(float angleDeg, FVector2D &vector);
	static void rotateVectorRad2D(float angleRad, FVector2D &vector);

	static float unsignedAngleRadBetween(FVector2D &a, FVector2D &b);
	static float signedAngleRadBetween(FVector2D &a, FVector2D &b);

	void transpose();

private:
	//16 langes array für die 4x4 matrix
	static constexpr int size = 16;
	float array[16];

	float clampDivisionByZero(float value);

	void swapIndices(int a, int b);

	float det(std::vector<float> &matrix);
	std::vector<float> collectExcept(int x, int y, std::vector<float> &matrix);

	static float signForAngle(FVector2D &a, FVector2D &b);


public:
	MMatrix jordanInverse();

	FVector lookDirXForward();

	void makeIdentity();

	FString toString();

private:
	void set(int i, int j, float value);
	float get(int column, int row);
	void scaleRow(int row, float scale);
	void minusForRow(int row, int otherRow, float faktor);

	
};

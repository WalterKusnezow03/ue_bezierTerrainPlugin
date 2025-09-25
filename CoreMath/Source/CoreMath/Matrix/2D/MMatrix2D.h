// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class COREMATH_API MMatrix2D
{
public:
	MMatrix2D();
	virtual ~MMatrix2D();

	MMatrix2D(FVector2D other);
	
	MMatrix2D(const MMatrix2D &other);
	MMatrix2D& operator=(const MMatrix2D &other);

	void SetTranslation(FVector2D vec);
	FVector2D getTranslation() const;

	void operator*=(const MMatrix2D & other);
	FVector2D operator*(const FVector2D &other) const;
	FVector2f operator*(const FVector2f &other) const;
	MMatrix2D operator*(const MMatrix2D &other) const;

	float extractAngle();

	void Scale(float scale);
	void Scale(float scaleX, float scaleY);

	void RadAdd(float angle);
	static float degToRadian(float deg);
	static float radToDegree(float rad);

	static MMatrix2D MakeScaleMatrixUniform(float scale);
	static MMatrix2D MakeScaleMatrix(float scaleX, float scaleY);

	void rotate(MMatrix2D &other);
	
	void transpose();
	void transposeRotationBlock();

	FString asString();

	bool IsZeroScaleMatrix() const;

	void SetRotation(const FVector2D &other);
	void SetRotation(const FVector2D &axis, const FVector2D &other);

private:
	//16 langes array für die 4x4 matrix
	static constexpr int size = 9;
	float array[9];


	void swapIndices(int a, int b);

public:

	void makeIdentity();
	void makeZero();

private:
	void set(int i, int j, float value);
	float get(int column, int row);
	
};

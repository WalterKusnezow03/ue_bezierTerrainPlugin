// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


/**
 * The data must be saved here to organize mesh actor layers for different materials 
 * and keeping code clean when applying meshes and handling more layers
 */
class P2_API MeshData
{
public:
	MeshData();
	~MeshData();

	MeshData(TArray<FVector> &&verteciesIn, TArray<int> &&trianglesIn);
	MeshData(const MeshData &other);
	MeshData &operator=(const MeshData &other);

	void append(MeshData &other);
	void append(
		FVector &a,
		FVector &b,
		FVector &c
	);
	void append(
		FVector &a,
		FVector &b,
		FVector &c,
		FVector &d
	);

	void appendEfficent(
		FVector &a,
		FVector &b,
		FVector &c
	);
	void appendEfficent(
		FVector &a,
		FVector &b,
		FVector &c,
		FVector &d
	);
	void appendEfficent(MeshData &other);


	void appendDoublesided(
		FVector &a,
		FVector &b,
		FVector &c
	);

	void appendDoublesided(
		FVector &a,
		FVector &b,
		FVector &c,
		FVector &d
	);

	void rebuild(TArray<FVector> &&verteciesIn, TArray<int> &&trianglesIn);

	void clearMesh();
	
	void setVertecies(TArray<FVector> &&verteciesIn);
	void setTriangles(TArray<int32> &&trianglesIn);

	void calculateNormals();

	TArray<FVector> &getVerteciesRef();
	TArray<int32> &getTrianglesRef();
	TArray<FVector> &getNormalsRef();
	TArray<FVector2D> &getUV0Ref();
	TArray<FProcMeshTangent> &getTangentsRef();
	TArray<FColor> &getVertexColorsRef();

	void offsetAllvertecies(FVector &offset);

	void appendVertecies(std::vector<FVector> &vec);

	void closeMeshAtCenter(FVector &center, std::vector<FVector> &vec, bool clockWise);
	void closeMeshAtCenter(FVector &center, int bufferSizeToConnect, bool clockWise);

	static std::vector<FVector> create2DQuadVertecies(int xMax, int yMax);


	//new helper
	void seperateMeshIntoAllTrianglesDoubleSided(std::vector<MeshData> &meshDataVectorOutput);
	void splitAllTrianglesInHalfAndSeperateMeshIntoAllTrianglesDoubleSided(
		std::vector<MeshData> &meshDataVectorOutput
	);

	FVector center();
	void centerMesh();

	void appendCube(
		FVector &a,
		FVector &b,
		FVector &c,
		FVector &d,
		FVector &a1,
		FVector &b1,
		FVector &c1,
		FVector &d1
	);

	void appendCube(
		FVector &a,
		FVector &b,
		FVector &c,
		FVector &d,
		FVector orthogonalDir
	);

private:
	float MIN_SPLITDISTANCE = 20.0f;
	bool canSplit(FVector &a, FVector &b, FVector &c);

	float EPSILON = 0.5f;
	bool isCloseSame(FVector &a, FVector &b);
	bool isCloseSame(FVector &a, int index);
	

	void fillUpMissingVertecies(int count);

	void clearNormals();

	void buildTriangle(
		FVector &a,
		FVector &b,
		FVector &c,
		TArray<FVector> &output,
		TArray<int32> &trianglesOutput
	);

	//mesh data i understand
	TArray<FVector> vertecies;
	TArray<int32> triangles;
	TArray<FVector> normals;

	TArray<FProcMeshTangent> Tangents; 
    TArray<FColor> VertexColors;

	//what ever these are
	TArray<FVector2D> UV0;

	int findClosestIndexTo(FVector &vertex);

	void join(TArray<FVector> &vertecies, TArray<int32> &triangles, TArray<FVector> &normalsin);

	bool isValidVertexIndex(int i);
	bool isValidTriangleIndex(int i);
	bool isValidNormalIndex(int index);
	FVector createNormal(int v0, int v1, int v2);
};

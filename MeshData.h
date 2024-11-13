// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "p2/meshgen/meshDataContainer/Quad.h"

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
	void append(Quad &&other);

	void clearMesh();
	void clearNormals();
	void setVertecies(TArray<FVector> &&verteciesIn);
	void setTriangles(TArray<int32> &&trianglesIn);
	void calculateNormals();

	TArray<FVector> &getVerteciesRef();
	TArray<int32> &getTrianglesRef();
	TArray<FVector> &getNormalsRef();
	TArray<FVector2D> &getUV0Ref();
	TArray<FProcMeshTangent> &getTangentsRef();
	TArray<FColor> &getVertexColorsRef();

	bool processHit(FVector &localHitpoint, FVector &direction);

private:
	//mesh data i understand
	TArray<FVector> vertecies;
	TArray<int32> triangles;
	TArray<FVector> normals;

	//what ever these are
	TArray<FVector2D> UV0;
    TArray<FProcMeshTangent> Tangents; 
    TArray<FColor> VertexColors;


	std::vector<Quad> quads;
	void join(TArray<FVector> &vertecies, TArray<int32> &triangles);
	void rebuildMeshDataFromQuads();

	void findClosestQuadsTo(
		FVector &localHitpoint,
		std::vector<int> &outputindices 
	);
};

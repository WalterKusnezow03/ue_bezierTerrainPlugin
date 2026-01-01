// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <set>
#include "GameCore/MeshGenBase/MeshData/BoundingBox/BoundingBoxSimple.h"
#include "KismetProceduralMeshLibrary.h"
#include "AssetEnumCollection/assetEnums/materialEnum.h"
#include "CoreMath/Matrix/MMatrix.h"

#include "GameCore/MeshGenBase/MeshData/intersectCache/TriangleIntersectFrame.h"


/**
 * The data must be saved here to organize mesh actor layers for different materials 
 * and keeping code clean when applying meshes and handling more layers
 */
class GAMECORE_API MeshData
{	
	// ------ SAVE TO STORAGE SECTION -------
public:


private:
	int internalLayerId = 0;

	// ------ MEMBER VARIABLES NEEDED FOR COPY CONSTRUCTOR ------
protected:
	//mesh data
	TArray<FVector> vertecies;
	TArray<int32> triangles;
	TArray<FVector> normals;
	TArray<FProcMeshTangent> Tangents; 
    TArray<FColor> VertexColors;
	TArray<FVector2D> UV0; //Uv generation custom planar mapping not implemented yet, to come

	//bound data needed for hittesting
	BoundingBoxSimple bounds;

	materialEnum materialPreferred = materialEnum::wallMaterial;



	// ------ CLASS SECTION --------
public:
	MeshData();
	virtual ~MeshData(); //automatischer aufruf aus subklasse

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

	///@brief tells if the triangle has 0 area and creates broken collision || 
	///useless drawcall overhead
	bool IsDegenerateTriangle(int a, int b, int c);

	//appendEfficentMehods will not handle uvs
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
	void appendEfficent(MeshData &other); //no uvs!
	void appendEfficentTriangleShapedBuffer(TArray<FVector> &verteciesIn);

	void MofidyEpsilon(float epsilon){
		EPSILON = std::max(1.0f, std::abs(epsilon));
	}

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

	void appendDoubleSidedTriangleBuffer(
		std::vector<FVector> &buffer
	);

	//uv append
	void appendUvs(
		FVector2D &a,
		FVector2D &b,
		FVector2D &c
	);
	void appendUvs(
		FVector2D &a,
		FVector2D &b,
		FVector2D &c,
		FVector2D &d
	);


	void appendUvsDoubleSided(
		FVector2D &a,
		FVector2D &b,
		FVector2D &c
	);
	void appendUvsDoubleSided(
		FVector2D &a,
		FVector2D &b,
		FVector2D &c,
		FVector2D &d
	);




	void rebuild(TArray<FVector> &&verteciesIn, TArray<int> &&trianglesIn);

	void clearMesh();
	
	void setVertecies(TArray<FVector> &&verteciesIn);
	void setTriangles(TArray<int32> &&trianglesIn);

	void calculateNormals();
	void flipNormals();
	void flipWindingOrder();

	TArray<FVector> &getVerteciesRef();
	TArray<int32> &getTrianglesRef();
	TArray<FVector> &getNormalsRef();
	TArray<FVector2D> &getUV0Ref();
	TArray<FProcMeshTangent> &getTangentsRef();
	TArray<FColor> &getVertexColorsRef();

	const TArray<FVector> &getVerteciesRefConst() const;
	const TArray<int32> &getTrianglesRefConst() const;
	const TArray<FVector> &getNormalsRefConst() const;
	const TArray<FVector2D> &getUV0RefConst() const;
	const TArray<FProcMeshTangent> &getTangentsRefConst() const;

	void offsetAllvertecies(FVector &offset);
	void transformAllVertecies(MMatrix &other);

	void appendVertecies(std::vector<FVector> &vec);
	

	void closeMeshAtCenter(FVector &center, std::vector<FVector> &vec, bool clockWise);
	void closeMeshAtCenter(FVector &center, int bufferSizeToConnect, bool clockWise);

	static std::vector<FVector> create2DQuadVertecies(int xMax, int yMax);


	//new helper
	void seperateMeshIntoAllTrianglesDoubleSided(std::vector<MeshData> &meshDataVectorOutput);
	void splitAllTrianglesInHalfAndSeperateMeshIntoAllTrianglesDoubleSided(
		std::vector<MeshData> &meshDataVectorOutput
	);
	void findLongestSide(
		FVector &a,
		FVector &b,
		FVector &c,
		FVector &startOut,
		FVector &dirOut
	);
	int findLongestSideIndex(
		FVector &a, 
		FVector &b, 
		FVector &c
	);
	

	void splitAndRemoveTrianglesAt(FVector &localHitPoint);
	bool doesHit(FVector &localHitPoint);

	FVector center();

	//centers the mesh around com
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

	void appendCubeAt(
		FVector &pivot,
		int scaleX,
		int scaleY,
		int scaleZ
	);

	void VerticalRangeOfBounds(float &a, float &b);

	void CreateCopyRecuriveDetailTo(MeshData &outData, int recursion);
	void CreateCopyRecuriveDetailToDistance(MeshData &outData, float distance);
	MeshData CreateCopyRecuriveDetail(int recursion);

protected:
	float MIN_SPLITDISTANCE = 50.0f;

	bool canSplit(int v0, int v1, int v2);
	bool canSplit(FVector &a, FVector &b, FVector &c);
	bool canSplit(FVector &a, FVector &b, FVector &c, float mindistanceKept);
	bool canSplit(int v0, int v1, int v2, float mindistanceKept);

	//splits edge along the edge index, returns the middle position inbetween
	FVector splitEdge(FVector &v0, FVector &v1, FVector &v2, int egdeIndex);
	void Split(FVector &v0, FVector &v1, FVector &v2, TArray<FVector> &outBuffer);
	void SplitTriangleShapedBuffer(
		TArray<FVector> &inBuffer, // must be triangle shaped
		TArray<FVector> &outBuffer
	);
	void SplitTriangleShapedBufferOverride(TArray<FVector> &buffer);

	float EPSILON = 5.0f;
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

	

	std::vector<int> findClosestIndexWithVertexDuplicatesTo(FVector &vertex);
	int findClosestIndexTo(FVector &vertex);
	int findClosestIndexToAndAvoid(FVector &vertex, int indexAvoid);
	int findClosestIndexToAndAvoid(FVector &vertex, std::vector<int> &avoid);

	void join(
		TArray<FVector> &vertecies, 
		TArray<int32> &triangles, 
		TArray<FVector> &normalsin, 
		TArray<FVector2D> &uvrefin,
		TArray<FTriangleIntersectFrame> &framesOther
	);

	bool isValidVertexIndex(int i);
	bool isValidVertexIndex(int i, int j, int n);
	bool isValidTriangleIndex(int i);
	bool isValidNormalIndex(int index);
	bool isValidNormalIndex(int i, int j, int k);
	FVector createNormal(int v0, int v1, int v2);

	void findTrianglesInvolvedWith(int index, std::vector<int> &trianglesFound);
	bool isPartOfTraingle(int target, int v0, int v1, int v2);

	bool solveIsInTriangle(
		int v0, int v1, int v2, FVector &target
	);
	

	void addTriangle(int v0, int v1, int v2);
	void addTriangleDoublesided(int v0, int v1, int v2);
	void splitTriangleInHalf(int v0, int v1, int v2);
	bool trianglesAreSame(int v0, int v1, int v2, int v00, int v01, int v02);
	int removeTriangleSimilarTo(int v0, int v1, int v2);

	//helper for removing triangles by vertex
	void removeVertex(int index);
	virtual void removeVertex(int index, std::vector<int> &connectedvertecies);
	void removeTrianglesInvolvedWith(int vertexIndex, std::vector<int> &connectedvertecies);
	bool contains(std::vector<int> &ref, int index);


public:
	
	materialEnum targetMaterial();
	void setTargetMaterial(materialEnum inMaterial);

	void generateMatricesPerFaceAndLookDirOfNormal(std::vector<MMatrix> &output);

	void generateMatricesPerFaceAndLookDirOfNormalInterpolated(
		std::vector<MMatrix> &output,
		int stepSize
	);

	int verteciesNum();
	bool hasAnyVertecies();

	//helper for displacement
	void pushInwards(FVector &location, int radius, FVector scaleddirection);

	void debugDrawMesh(MMatrix &transform, UWorld *world);

	void flipAllTriangles();

	/// @brief update the bounding box if vertex data was manipulated used externally
	void updateBoundsIfNeeded();

protected:
	/// @brief finds all indices connected in the triangle buffer to this index and
	/// pushed into the output vector
	/// @param index some index
	/// @param output output vector
	void findConnectedVerteciesTo(int index, std::vector<int> &output);




	
	void updateBoundsIfNeeded(FVector &other);

	bool isInsideBoundingbox(FVector &other);

	float AverageDist2TriangleShapedBuffer(TArray<FVector> &buffer);

	/**
	 * 2D section
	 */
public:
	void generate(int sizeX, int sizeY, int distanceXY);

	FVector &findIndex(int i, int j);
	void replaceAt(int i, int j, FVector &other);

protected:
	int indexFor(int i, int j);

	FVector noneVertex;
	int umbruch = 0;



/**
 * road section
 */
public:
	void appendParalellLinesClosedAsQuads(
		TArray<FVector> &line1,
		TArray<FVector> &line2
	);



/**
 * Intersection tests
 */
public:
	bool RayIntersect(
		const FVector &origin,
		const FVector &direction,
		FVector &outIntersectionPoint
	);

protected:
	bool RayIntersectBounds(const FVector &origin, const FVector &direction);
	void AppendIntersectionFrame(int32 v0Index, int32 v1Index, int32 v2Index);

	void RebuildAllIntersectFrames();
	void RefreshAllTriangleFramesWith(int32 index);
	void RefreshTriangleFrame(int32 v0Index, int32 v1Index, int32 v2Index);

	void RemoveAllTriangleFramesWithIndex(int32 vIndex);
	void RemoveTriangleFrame(int32 v0Index, int32 v1Index, int32 v2Index);
	bool AlreadyHasTriangleFrame(int32 v0Index, int32 v1Index, int32 v2Index);

private:
	TArray<FTriangleIntersectFrame> intersectFrames;
};

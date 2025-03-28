// Fill out your copyright notice in the Description page of Project Settings.


#include "roomProcedural.h"
#include "p2/util/TTouple.h"
#include "p2/DebugHelper.h"
#include "p2/util/FVectorUtil.h"
#include "p2/entities/customIk/MMatrix.h"
#include "p2/gamestart/assetManager.h"
#include "p2/entityManager/EntityManager.h"
#include "p2/_world/worldLevel.h"
#include "p2/rooms/testing/helper/staircaseBoundData.h"
#include "p2/meshgen/foliage/ETerrainType.h"
#include "p2/util/FVectorUtil.h"
#include "p2/rooms/layoutCreator/layoutMaker.h"

template class TTouple<FVector, FVector>;

// Sets default values
AroomProcedural::AroomProcedural()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AroomProcedural::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AroomProcedural::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void AroomProcedural::createRoom(
	FVector location,
	roomBoundData &currentRoom,
	int oneMeter
){
	SetActorLocation(location);

	/*
	createRoom(
		fullOffset,
		currentRoom,
		oneMeter,
		false, //open for stairs
		true
	);*/

	//reset height because mesh starts at 0 height, world location is set by actor itself
	location.Z = 0.0f;
	int layers = FVectorUtil::randomNumber(1, 3);
	for (int i = 0; i < layers; i++){
		bool openForStaircaseBottom = (i != 0);
		bool openForStaircaseTop = (i != layers - 1);

		createRoom(
			location,
			currentRoom,
			oneMeter,
			openForStaircaseBottom,
			openForStaircaseTop
		);
	}

	//very important to reload the mesh
	ReloadMeshAndApplyAllMaterials();
}







int AroomProcedural::zScaleInCentimeters(){
	return 3 * 100;
}


///@brief creates the room and automatically adjusts @param location for the next layer, adds
///zcm to the vector! All meshes are transformed as expected in the next layer if calling the function again!
void AroomProcedural::createRoom(
	FVector &location, //bottom left corner, with heightOffset
	roomBoundData &currentRoom,
	int onemeter,
	bool openForStaircaseBottom,
	bool openForStaircaseTop
){

	int scaleMetersX = currentRoom.xScale();
	int scaleMetersY = currentRoom.yScale();
	
	std::vector<FVector> doorPositions = currentRoom.relativeDoorPositionsCm(
		onemeter); // in local space to bottom left corner
	std::vector<FVector> windowPositions = currentRoom.relativeWindowPositionsCm(
		onemeter); // in local space to bottom left corner

	int zCm = zScaleInCentimeters();
	int windowWidthCm = onemeter;
	int doorWidthCm = onemeter;

	// erstmal den boden
	MeshData floorAndRoof;
	MeshData walls;
	MeshData windows;

	FVector bottomLeftAnchor;

	currentRoom.appendBottomOrTopClosed(
		floorAndRoof,
		openForStaircaseBottom,
		onemeter,
		0.0f //zCm
	);
	currentRoom.appendBottomOrTopClosed(
		floorAndRoof,
		openForStaircaseTop,
		onemeter,
		zCm//zCm
	);
	//create staircase if to top needed.
	if(openForStaircaseTop){
		createStairs(
			floorAndRoof,
			currentRoom,
			onemeter
		);
	}






	//floor
	std::vector<FVector> quad = MeshData::create2DQuadVertecies(scaleMetersX * onemeter, scaleMetersY * onemeter);
	
	//dann wände --> die man mit einer methode schreibt
	//türen nach wänden sortieren
	//std::vector<FVector> corners = {bl, tl, tr, br, bl};
	std::vector<FVector> corners = {quad[0], quad[1], quad[2], quad[3], quad[0]};


	for (int i = 1; i < corners.size(); i++)
	{
		FVector &from = corners[i - 1];
		FVector &to = corners[i];
		createWall(
			walls,
			windows,
			from, 
			to, 
			doorPositions,
			windowPositions, 
			doorWidthCm, 
			zCm
			//,
			//location
		);
	}


	//stairs debug
	//DebugCreateStairs(floorAndRoof);

	//testing from here needed to spawn the room / apply mesh(es)
	floorAndRoof.calculateNormals();
	walls.calculateNormals();

	//offset
	FVector goUp(0, 0, location.Z);
	MMatrix translateOffset(goUp);
	floorAndRoof.transformAllVertecies(translateOffset);
	walls.transformAllVertecies(translateOffset);
	windows.transformAllVertecies(translateOffset);

	/*
	void appendMeshDataAndReload(
		MeshData &meshdata,
		materialEnum type,
		ELod lodLevel,
		bool raycastOnLayer
	);
	*/

	bool raycastOnLayer = true;
	appendMeshDataAndReload(
		floorAndRoof,
		materialEnum::stoneMaterial,
		ELod::lodNear,
		raycastOnLayer
	);
	appendMeshDataAndReload(
		walls,
		materialEnum::wallMaterial,
		ELod::lodNear,
		raycastOnLayer
	);
	appendMeshDataAndReload(
		windows,
		materialEnum::glassMaterial,
		ELod::lodNear,
		raycastOnLayer
	);



	//go to next layer
	location.Z += zCm;
}

void AroomProcedural::createWall(
	MeshData &wallMesh,
	MeshData &windowMesh,
	FVector from, 
	FVector to, 
	std::vector<FVector> &doors, //doors in cm local
	std::vector<FVector> &windows, //windows in cm local
	int doorWidthCm,
	int scaleZCm
){
	//sort vectors for consistent door placement, otherwise overlap issues occur
	//very important!
	FVector connect = to - from;
	if(connect.X < 0 || connect.Y < 0){
		FVector copy = to;
		to = from;
		from = copy;
	}
	

	std::vector<FVector> oneDimWall;
	oneDimWall.push_back(from); //FIRST WALL POSITION


	std::vector<FVector> doorsFiltered;
	filterForVectorsBetween(
		from,
		to,
		doorWidthCm,
		doors,
		doorsFiltered //output
	);

	std::vector<FVector> windowsFiltered;
	filterForVectorsBetween(
		from,
		to,
		doorWidthCm,
		windows,
		windowsFiltered //output
	);
	
	// create gaps for wall
	createGapsFor(from, to, doorWidthCm, windowsFiltered, oneDimWall);
	createGapsFor(from, to, doorWidthCm, doorsFiltered, oneDimWall);
	//last part added here, wall "complete", lower ceiling 
	oneDimWall.push_back(to); //FINAL POSITION
	sortVectorsBetween(from, to, oneDimWall);
	

	//walls new
	//MeshData output;

	int widthCmWall = 20;
	appendWallsFromMeshBounds(
		wallMesh,
		oneDimWall,
		widthCmWall,
		scaleZCm
	);

	// --- CREATE WINDOWS ---
	
	//will save the 1d window representation
	std::vector<FVector> oneDimWindows;

	createGapsFor(
		from,
		to,
		doorWidthCm,
		windowsFiltered,
		oneDimWindows //output
	);

	//simplify windows
	removeCloseTouples(oneDimWindows);

	//spawn windows seperately
	/*
	spawnWindowMeshFromBounds(
		oneDimWindows,
		scaleZCm,
		5.0f, //5cm
		locationOffset
	);*/

	//spawn windows part of this mesh Actor
	appendWindowsFromMeshBounds(
		windowMesh,
		oneDimWindows,
		scaleZCm
	);

}











void AroomProcedural::createGapsFor(
	FVector &start,
	FVector &end,
	float width,
	std::vector<FVector> &inputPositions,
	std::vector<FVector> &output
){
	for (int i = 0; i < inputPositions.size(); i++){
		FVector &current = inputPositions[i];

		createGapAt(
			start,
			end,
			current,
			width,
			output
		);
	}
}






void AroomProcedural::createGapAt(
	FVector &start,
	FVector &end,
	FVector &locationStart,
	float width,
	std::vector<FVector> &output
){
	// create gaps for wall
	FVector direction_to = (end - start).GetSafeNormal() * width; //verlängerungs skalar
	direction_to.Z = 0;

	FVector A = locationStart;
	FVector B = locationStart + direction_to;
	output.push_back(A);
	output.push_back(B);
}


void AroomProcedural::appendWallsFromMeshBounds(
	MeshData &output,
	std::vector<FVector> &vec,
	int widthCmWall,
	int scaleZCm
){
	for (int i = 1; i < vec.size(); i += 2){ //immer one skip das die läcken der fenster auch da sind.
		FVector &currentStart = vec[i - 1];
		FVector &currentEnd = vec[i];

		if(FVector::Dist(currentStart, currentEnd) > 10){
			appendCubeTo(
				output,
				currentStart,
				currentEnd,
				std::abs(scaleZCm),
				widthCmWall
			);
		}
	}
}

//new helper method, merge windows into all meshdata from this actor
void AroomProcedural::appendWindowsFromMeshBounds(
	MeshData &output,
	std::vector<FVector> &vec,
	int scaleZCm
){
	FVector upVec(0, 0, std::abs(scaleZCm));
	for (int i = 1; i < vec.size(); i += 2){ //immer one skip, paar weise
		FVector &v0 = vec[i - 1];
		FVector &v3 = vec[i];
		FVector v1 = v0 + upVec;
		FVector v2 = v3 + upVec;

		output.appendDoublesided(v0, v1, v2, v3);
	}
}








void AroomProcedural::appendCubeTo(
	MeshData &outputData,
	FVector &start,
	FVector &end,
	float height,
	float width
){
	FVector offsetZ(0, 0, height);
	FVector offsetWidth(width, 0, 0); //x is forward in MMatrix, create rotator from start to end

	FVector start2D = start;
	start2D.Z = 0.0f;
	FVector end2D = end;
	end2D.Z = 0.0f;
	FVector connect = end2D - start2D;

	MMatrix rotator = MMatrix::createRotatorFrom(connect);
	rotator.yawRadAdd(MMatrix::degToRadian(90.0f));

	offsetWidth = rotator * offsetWidth; //eindrehen

	/*
	1->2
	0<-3
	*/
	FVector a = start;
	FVector b = end;
	FVector c = end + offsetWidth;
	FVector d = start + offsetWidth;

	FVector a1 = a + offsetZ;
	FVector b1 = b + offsetZ;
	FVector c1 = c + offsetZ;
	FVector d1 = d + offsetZ;


	outputData.appendCube(a, b, c, d, a1, b1, c1, d1);

}


//deprecated.

/// @brief creates a new mesh from touples representing the vertical edges of an window
/// @param windowTouples 
/// @param offset 
void AroomProcedural::spawnWindowMeshFromBounds(
	std::vector<FVector> &windowTouples,
	float scaleZCm,
	float width,
	FVector &offset
){

	assetManager *assetManagerPointer = assetManager::instance();
	EntityManager *e = worldLevel::entityManager();
	if(assetManagerPointer != nullptr && e != nullptr){
		for (int i = 1; i < windowTouples.size(); i += 2){

			AcustomMeshActor *newActor = e->spawnAcustomMeshActor(GetWorld(), offset);
			if(newActor != nullptr){
				
				newActor->SetActorLocation(offset);

				MeshData &glassMesh = newActor->findMeshDataReference(
					materialEnum::glassMaterial,
					ELod::lodNear,
					true
				);

				FVector currentStart = windowTouples[i - 1];
				FVector currentEnd = windowTouples[i];

				FVector v1 = currentStart + FVector(0, 0, std::abs(scaleZCm));
				FVector v2 = currentEnd + FVector(0, 0, std::abs(scaleZCm));

				glassMesh.appendDoublesided(currentStart, v1, v2, currentEnd);
				//glassMesh.append(currentStart, v2, v1, currentEnd); //debug.
				/*
				appendCubeTo(
					glassMesh, //output
					currentStart,
					currentEnd,
					std::abs(scaleZCm),
					width
				);*/

				newActor->ReloadMeshAndApplyAllMaterials();

				bool splitGlass = true;
				newActor->setMaterialBehaiviour(materialEnum::glassMaterial, splitGlass);
			}
		}
	}

}

///@brief expects the passed vector to be in touples of bounds M := {(a,b),(a,b)...}
/// generates a minimized vector and replaces the passed reference data!
void AroomProcedural::removeCloseTouples(
	std::vector<FVector> &vec
){
	if(vec.size() <= 2){
		return;
	}

	//TESTING NEEDED
	std::vector<FVector> simplified;
	std::vector<bool> flaggedSkip(vec.size(), false);
	for (int i = 2; i < vec.size(); i++)
	{
		FVector &prev = vec[i-1];
		FVector &current = vec[i];
		
		//if too close, remove both!
		float dist = FVector::Dist(prev, current);
		if(dist <= 1.0f){
			flaggedSkip[i - 1] = true;
			flaggedSkip[i] = true;
		}
	}

	//start and end never removed
	if(flaggedSkip.size() > 0){
		flaggedSkip[0] = false;
		flaggedSkip[flaggedSkip.size() - 1] = false;
	}

	//add not ignored flags
	for (int i = 0; i < flaggedSkip.size(); i++){
		if(flaggedSkip[i] == false){
			if(i >= 0 && i < vec.size()){
				simplified.push_back(vec[i]);
			}
		}
	}


	vec = simplified; //copy
}

/// @brief will filter positions from the positions to filter vector
/// and add the positions which meet the requirement into the filtered list
/// @param A start
/// @param B end
/// @param positionsToFilter any vectors
/// @param output vectors between a and b / paralell to AB
void AroomProcedural::filterForVectorsBetween(
	FVector &A,
	FVector &B,
	int minDistance,
	std::vector<FVector> &positionsToFilter,
	std::vector<FVector> &output
){
	FVector zeroVec(0, 0, 0);
	FVector AB = (B - A).GetSafeNormal();
	FVector BA = (A - B).GetSafeNormal();

	minDistance = std::abs(minDistance);

	float distanceBetweenAB = FVector::Dist(A, B);

	//minDistance *= 0.9f;

	
	for (int i = 0; i < positionsToFilter.size(); i++){
		FVector &current = positionsToFilter[i];
		FVector AC = (current - A); //for distance
		FVector ACdir = AC.GetSafeNormal(); //direction

		//vector from A to B and A to wall will be paralell if the wall is in interest
		float dot = AB.X * ACdir.X + AB.Y * ACdir.Y;
		
		//ist parallell und ggf zwischen den beiden positionen
		if(dot >= 0.99f){

			FVector BC = (current - B);
			float distanceToStartPoint = lenghtOf(AC);
			float distanceToEndPoint = lenghtOf(BC);

			if(
				distanceToStartPoint >= minDistance && //distanze minimal
				distanceToEndPoint >= minDistance &&
				distanceToStartPoint < distanceBetweenAB && //distanz kleiner als maximal sodass innen
				distanceToEndPoint < distanceBetweenAB
			){
				output.push_back(current);
			}

		}
	}


	
}

float AroomProcedural::lenghtOf(FVector &vec){
	FVector zeroVec(0, 0, 0);
	return std::abs(FVector::Dist(zeroVec, vec));
}


void AroomProcedural::sortVectorsBetween(FVector &A, FVector &B, std::vector<FVector> &output){
	FVector AB = B - A;

	bool vectorIsXDirection = AB.X != 0; //ansonsten Z
	bool isNegativeToPositive = true;
	if(vectorIsXDirection){
		isNegativeToPositive = AB.X > 0; //from 0 to 8 for example direction
	}else{
		isNegativeToPositive = AB.Y > 0; //from 0 to 8 for example direction
	}


	std::sort(
		output.begin(), output.end(),
		[vectorIsXDirection, isNegativeToPositive]
		(const FVector &a, const FVector &b) {
		if(vectorIsXDirection){
			return a.X < b.X;
		}else{
			return a.Y < b.Y;
		}
	});

	//debug print sorted vector
	DebugHelper::logMessage("debug vector sorted: ");
	for (int i = 0; i < output.size(); i++)
	{
		int number = vectorIsXDirection ? output.at(i).X : output.at(i).Y;
		FString s = FString::Printf(TEXT("debug vector sorted %d"), number);
		DebugHelper::logMessage(s); // sieht gut aus
	}
}















/**
 * static method section
 */

/// @brief spawns a vector of roomBound data into the world
/// @param worldIn world to spawn in
/// @param location location to spawn at
/// @param vec vector of rooms
void AroomProcedural::spawnRooms(
	UWorld* worldIn, 
	FVector location, 
	std::vector<roomBoundData> &vec,
	ETerrainType terraintype
){
	if(worldIn == nullptr){
		return;
	}

	//process all rooms to be created
	for (int i = 0; i < vec.size(); i++){
		roomBoundData &currentRoom = vec.at(i);
		//create proper offset in xpos and ypos as needed

		int onemeter = 100;

		FVector fullOffset = location + currentRoom.positionInMeterSpace(onemeter);
		AroomProcedural *newRoom = spawnRoom(worldIn, fullOffset);
		if(newRoom != nullptr){

			newRoom->updateTerrainTypeLocatedIn(terraintype);

			newRoom->createRoom(
				fullOffset,
				currentRoom,
				onemeter
			);

		}

	}
}

void AroomProcedural::updateTerrainTypeLocatedIn(ETerrainType input){
	locatedInTerrainType = input;
}


AroomProcedural* AroomProcedural::spawnRoom(UWorld *world, FVector location){
	if(world == nullptr){
		return nullptr;
	}

	FActorSpawnParameters params;
	AroomProcedural *spawned = world->SpawnActor<AroomProcedural>(
		AroomProcedural::StaticClass(),
		location,
		FRotator::ZeroRotator,
		params
	);
	return spawned;
}










/**
 * 
 * 
 * 
 * ----- new testing section fvector shape / tree alike generation 
 * planned here
 * 
 * 
 * 
 */

///@brief generates a room. CAUTION: size in is METERS
void AroomProcedural::generate(UWorld *world, int sizeXMeters, int sizeYMeters, FVector location){
	generate(
		world,
		sizeXMeters,
		sizeYMeters,
		location,
		ETerrainType::ETropical
	);
}



void AroomProcedural::generate(
	UWorld *world, 
	int sizeXMeters, 
	int sizeYMeters, 
	FVector location,
	ETerrainType terraintype
){

	std::vector<TTouple<int, int>> sizesPossible;
    sizesPossible.push_back(TTouple<int, int>(sizeXMeters / 2, sizeYMeters / 2));
    sizesPossible.push_back(TTouple<int, int>(sizeXMeters / 3, sizeYMeters / 3));
    sizesPossible.push_back(TTouple<int, int>(sizeXMeters / 4, sizeYMeters / 4));

    std::vector<roomBoundData> outputRooms;
    layoutMaker l;
    l.makeLayout(sizeXMeters, sizeYMeters, sizesPossible, outputRooms);
    AroomProcedural::spawnRooms(world, location, outputRooms, terraintype);

}







//new expiremental
void AroomProcedural::DebugCreateStairs(MeshData &appendTo){
	int size = 3;
	StaircaseBoundData stairs;
	stairs.createLayout(size, size);

	//generate(int oneMeter, int heightMeters, int maxSlopeMeters)
	MeshData stairData = stairs.generate(120, 300, 100);


	appendTo.appendEfficent(stairData);
}




void AroomProcedural::createStairs(
	MeshData &data,
	roomBoundData &room,
	int onemeter
){
	//"returns the bounds in INDEX space"
	std::vector<FVector> &ref = room.stairBoundsQuadInIndexSpaceRef(onemeter);
	if(ref.size() == 4){
		/*
		1 2 
		0 3 
		*/
		int indexDistX = std::abs(ref[3].X - ref[0].X);
		int indexDistY = std::abs(ref[1].Y - ref[0].Y);

		StaircaseBoundData stairs;
		stairs.createLayout(indexDistX, indexDistY);

		//generate(int oneMeter, int heightMeters, int maxSlopeMeters)
		MeshData stairData = stairs.generate(onemeter, 300, 90);

		FVector tinMeter = ref[0] * onemeter;
		MMatrix translate(tinMeter);
		stairData.transformAllVertecies(translate);

		data.appendEfficent(stairData);
	}
}
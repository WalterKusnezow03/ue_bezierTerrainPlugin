// Fill out your copyright notice in the Description page of Project Settings.


#include "customWaterActor.h"
#include "p2/gamestart/assetManager.h"
#include "p2/meshgen/lodHelper/LodCheckContainer.h"
#include "p2/entities/customIk/MMatrix.h"
#include "p2/_world/worldLevel.h"
#include "ripple.h"


AcustomWaterActor::AcustomWaterActor() : AcustomMeshActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    meshInited = false;
    setTeam(teamEnum::neutralTeam);
}

void AcustomWaterActor::BeginPlay(){
    Super::BeginPlay();
    meshInited = false;
    setTeam(teamEnum::neutralTeam);
}


void AcustomWaterActor::Tick(float DeltaTime){
    Super::Tick(DeltaTime);

    if(!worldLevel::terrainIsInitedFlag()){
        return;
    }

    if(!meshInited){
        return;
    }

    if (playerIsInRenderRange()){
        //updateRunningTime(DeltaTime);
        TickRipples(DeltaTime); //tick ripples before vertex shader to already modify mesh
        vertexShader();
    }
}


void AcustomWaterActor::createWaterPane(
    UWorld *world, 
    FVector location,
    int scaleMeters
){
    if(world == nullptr){
        return;
    }
    if(scaleMeters < 1){
        return;
    }

    int vertexCount = scaleMeters / DEFAULT_DISTANCE_BETWEEN_VERTECIES;

    int paneCount = vertexCount / MAX_VERTEXCOUNT;
    if(paneCount == 0){ //below max size, do 1
        paneCount = 1;
    }

    int offsetOnAxis = MAX_VERTEXCOUNT * DEFAULT_DISTANCE_BETWEEN_VERTECIES;

    DebugHelper::logMessage("DEBUGSIZE OF WATER VERTEXCOUNT", vertexCount);
    DebugHelper::logMessage("DEBUGSIZE OF WATER PANECOUNT", paneCount);

    FVector offsetVector(0,0,100);
    offsetVector += location;

    for (int i = 0; i < paneCount; i++){
        for (int j = 0; j < paneCount; j++){

            FVector finalLocation = offsetVector;
            offsetVector.X += offsetOnAxis * i;
            offsetVector.Y += offsetOnAxis * j;

            FRotator rotation;
            FActorSpawnParameters params;
            AcustomWaterActor *SpawnedActor = world->SpawnActor<AcustomWaterActor>(
                AcustomWaterActor::StaticClass(),
                finalLocation,
                FRotator::ZeroRotator,
                params
            );
            if(SpawnedActor != nullptr){
                SpawnedActor->createWaterPane(
                    vertexCount, 
                    DEFAULT_DISTANCE_BETWEEN_VERTECIES
                );
            }
        }
    }
}




/// @brief 
/// @param sizeX in vertecies
/// @param sizeY in vertecies
/// @param detail detail between vertecies (in cm)
void AcustomWaterActor::createWaterPane(int vertexCountIn, int detail){
    if(meshInited){
        return;
    }

    vertexCountIn = std::abs(vertexCountIn);
    if(vertexCountIn <= 2){
        vertexCountIn = 3;
    }
    if(vertexCountIn > MAX_VERTEXCOUNT){
        vertexCountIn = MAX_VERTEXCOUNT;
    }

    vertexcountX = vertexCountIn;
    vertexcountY = vertexCountIn;

    //VERY IMPORTANT
    BottomLeft = FVector(0,0,0);
	TopLeft = FVector(0, vertexCountIn * detail, 0);
	BottomRight = FVector(vertexCountIn * detail,0,0);
	TopRight = FVector(vertexCountIn * detail,vertexCountIn * detail,0);

    ownHalfSize = (vertexCountIn * detail) / 2.0f;

    MeshData &waterMesh = findMeshDataReference(
        materialEnum::waterMaterial,
        ELod::lodNear
    );

    for (int i = 0; i < vertexcountX; i++){
        for (int j = 0; j < vertexcountY; j++){
            int xOffset = i * detail;
            int yOffset = j * detail;
            /*
            1 2
            0 3
            */
            FVector v0 = FVector(xOffset, yOffset, 0);
            FVector v1 = FVector(xOffset, yOffset + detail, 0);
            FVector v2 = FVector(xOffset + detail, yOffset + detail, 0);
            FVector v3 = FVector(xOffset + detail, yOffset, 0);

            waterMesh.appendEfficent(v0, v1, v2, v3);
        }
    }
    waterMesh.calculateNormals();

    ReloadMeshAndApplyAllMaterials();
    meshInited = true;


    //exclude this for bone controller raycast
    EntityManager *entityManagerPointer = worldLevel::entityManager();
    if(entityManagerPointer){
        entityManagerPointer->addActorToIgnoredAllParams(this);
    }


    //update collsion params for this mesh after it was setup
    UProceduralMeshComponent *thisMesh = meshComponentPointer();
    if (thisMesh)
    {
        thisMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        thisMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
        thisMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

    }
    
}

MeshData& AcustomWaterActor::findMeshDataReference(
    materialEnum mat,
    ELod lod
){
    return Super::findMeshDataReference(
        mat,
        lod,
        true // mesh
    );
}

UProceduralMeshComponent* AcustomWaterActor::meshComponentPointer(){
    if(Mesh){
        return Mesh;
    }
    return nullptr;
}


void AcustomWaterActor::vertexShader(){
    MeshData &waterMesh = findMeshDataReference(
        materialEnum::waterMaterial,
        ELod::lodNear
    );

    int layer = layerByMaterialEnum(materialEnum::waterMaterial);

    FVector locationOfPlayer = playerLocation();

    //raycast is not blocked by water
    UProceduralMeshComponent *thisMesh = meshComponentPointer();
    if(thisMesh){
        TArray<FVector> &vertecies = waterMesh.getVerteciesRef();

        //old working, linear: bad
        
        FVector actorLocation = GetActorLocation();
        for (int i = 0; i < vertecies.Num(); i++)
        {
            if(i >= 0 && i < vertecies.Num()){
                FVector &vertex = vertecies[i];

                if(!isAtLockedAxis(vertex)){
                    applyShaderToVertex(vertex);
                    applyWaterRippleOffset(vertex, actorLocation);
                }else{
                    //debug draw shows it should be fine, somewhere the flag gets resettet
                    /*
                    DebugHelper::showLineBetween(
                        GetWorld(),
                        GetActorLocation() + vertex,
                        GetActorLocation() + vertex + FVector(0, 0, 1000),
                        FColor::Orange,
                        0.1f
                    );*/
                    resetVertexShadignFor(vertex);
                }
            }
            
        }

        refreshMesh(*thisMesh, waterMesh, layer);
    }
}

///@brief will tell whether a vertex should be moved or not
bool AcustomWaterActor::isAtLockedAxis(FVector &other){
    //check for axis
    int buffer = 100;
    if (topAxisLocked)
    {
        if(other.Y + buffer >= TopRight.Y){
            return true;
        }
    }
    if(bottomAxisLocked){
        if(other.Y - buffer <= BottomRight.Y){
            return true;
        }
    }
    if(rightAxisLocked){
        if(other.X + buffer >= BottomRight.X){
            return true;
        }
    }
    if(leftAxisLocked){
        if(other.X - buffer <= BottomLeft.X){
            return true;
        }
    }
    return false;
}


/// @brief apply vertex shader to the given vertex
/// @param vertex vertex to move
void AcustomWaterActor::applyShaderToVertex(FVector &vertex){
    FVector actorLocation = GetActorLocation();
    float distXAll = vertex.X + actorLocation.X;
    float distYAll = vertex.Y + actorLocation.Y;

    float frequency = 0.01f; // Wellenbreite
    float amplitude = 10.0f; // Wellenhöhe
    float speed = 1.0f; // Wellengeschwindigkeit //1.0;
    float wave = sin(distXAll * frequency + shaderRunningTime * speed) + 
                 cos(distYAll * frequency + shaderRunningTime * speed);

    vertex.Z = wave * amplitude;

}


void AcustomWaterActor::resetAllShaderOffsets(){
    UProceduralMeshComponent *thisMesh = meshComponentPointer();
    if(thisMesh){

        MeshData &waterMesh = findMeshDataReference(
            materialEnum::waterMaterial,
            ELod::lodNear
        );
    
        int layer = layerByMaterialEnum(materialEnum::waterMaterial);
    
        TArray<FVector> &verteciesReference = waterMesh.getVerteciesRef();
        for (int i = 0; i < verteciesReference.Num(); i++){
            resetVertexShadignFor(verteciesReference[i]);
        }

        refreshMesh(*thisMesh, waterMesh, layer);
    }
}

void AcustomWaterActor::resetVertexShadignFor(FVector &other){
    other.Z = 0.0f;
}

void AcustomWaterActor::refreshMesh(
    UProceduralMeshComponent& meshComponent,
    MeshData &other,
    int layer
){
    if(meshInited){
        Super::refreshMesh(meshComponent, other, layer);
    }
}




/**
 * -- damage interaction --
 */
void AcustomWaterActor::setTeam(teamEnum t){
    teamSaved = t;
}

teamEnum AcustomWaterActor::getTeam(){
    return teamSaved;
}

void AcustomWaterActor::takedamage(int d){
    //nicht reagieren
}

void AcustomWaterActor::takedamage(int d, FVector &hitpoint){
    //DebugHelper::logMessage("water damage");
    //return;
    addNewRipple(hitpoint);
}

/**
 * 
 * -- ripple index management --
 * 
*/
void AcustomWaterActor::TickRipples(float DeltaTime){
    std::vector<int> markedForRemoval;
    for (int i = 0; i < rippleVecSize; i++)
    {
        if(rippleIndexIsValid(i)){
            ripple &current = rippleVector[i];
            current.Tick(DeltaTime);
            if (current.timeExceeded())
            {
                markedForRemoval.push_back(i);
            }
        }
    }

    for (int i = 0; i < markedForRemoval.size(); i++){
        removeRippleAtIndex(markedForRemoval[i]);
    }
}

void AcustomWaterActor::applyWaterRippleOffset(FVector &vertex, FVector &actorLocation){
    float offsetSum = 0.0f;
    for (int i = 0; i < rippleVecSize; i++)
    {
        if(rippleIndexIsValid(i)){
            ripple &current = rippleVector[i];
            current.changeHeightBasedOnDistance(vertex, actorLocation);
        }
    }
}

///@brief creates a new or reuses an old ripple object from the local object pool
void AcustomWaterActor::addNewRipple(FVector &location){
    if(rippleVector.size() == 0 || rippleVecSize >= rippleVector.size()){
        rippleVector.push_back(ripple(location, ownHalfSize)); //ownHalfSize for max radius
        rippleVecSize = rippleVector.size();
    }
    
    if(rippleVecSize < rippleVector.size()){
        int last = rippleVecSize;
        rippleVector[last].init(location, ownHalfSize * 0.5f); //reuse object
        rippleVecSize++;
    }

}


bool AcustomWaterActor::rippleIndexIsValid(int index){
    return index >= 0 && index < rippleVecSize && index < rippleVector.size();
}

void AcustomWaterActor::removeRippleAtIndex(int index){
    if(rippleVector.size() == 0){
        rippleVecSize = 0;
        return;
    }

    if(rippleIndexIsValid(index)){
        int preLast = rippleVecSize - 1;
        if(preLast == index){
            rippleVecSize--;
            return;
        }

        if(rippleIndexIsValid(preLast)){
            //copy "back" to index and hide with seperate end pointer
            rippleVector[index] = rippleVector[preLast];
            rippleVecSize--;
        }
    }
}




/**
 * helper player distance
 */

bool AcustomWaterActor::playerIsInRenderRange(){

    referenceManager *manager = referenceManager::instance();
    if(manager != nullptr){

        FVector locationOfPlayer = playerLocation();

        FVector actorLocation = GetActorLocation();
        LodCheckContainer checkContainer;
        checkContainer.modifyUpperDistanceLimitFor(ELod::lodNear, 50*100); //50
        checkContainer.modifyUpperDistanceLimitFor(ELod::lodMiddle, 200*100);
        checkContainer.checkLod(actorLocation, locationOfPlayer);
        SetActorHiddenInGame(checkContainer.hideActorByLod()); //if far, hide


        //remove vertex displacement if changed to middle
        ELod lodResult = checkContainer.lod();
        ELod prevLod = latestLodMeasured;
        latestLodMeasured = lodResult;

        if(lodResult == ELod::lodMiddle || lodResult == ELod::lodFar){
            if(prevLod != lodResult){
                resetAllShaderOffsets(); //remove vertex displacement ONCE
            }
            return false;
        }

        //check for edge case near to middle
        if(lodResult == ELod::lodNear){
            
            if(checkContainer.lodWasEdgeCaseToNextLod()){ //was very near to middle case

                //do vertex alignment at end to have none
                FVector playerLook = manager->playerLookDir();
                
                //lock according rows to player look to actor location
                lockOuterAxisBasedOn(
                    locationOfPlayer,
                    playerLook
                );
            }else{
                unlockAllAxis();
            }

            return true;   
        }

    }
    return false;
}


/**
 * locking axis
 */

void AcustomWaterActor::unlockAllAxis(){
    topAxisLocked = false;
	bottomAxisLocked = false;
	leftAxisLocked = false;
	rightAxisLocked = false;
}

void AcustomWaterActor::lockOuterAxisBasedOn(
    FVector &playerLocation,
    FVector &playerLookDir
){
    unlockAllAxis();
    FVector playerInLocalSpace = playerLocation - GetActorLocation();
    playerInLocalSpace.Z = 0.0f;
    playerLookDir = playerLookDir.GetSafeNormal();
    playerLookDir.Z = 0.0f;


    //koordinaten eindrehen, dann messen
    MMatrix playerRotator = MMatrix::createRotatorFrom(playerLookDir);
    FVector topRightRotated = playerRotator * TopRight;
    FVector bottomRightRotated = playerRotator * BottomRight;
    FVector topLeftRotated = playerRotator * TopLeft;
    
    //2D distance
    topRightRotated.Z = 0.0f;
    bottomRightRotated.Z = 0.0f;
    topLeftRotated.Z = 0.0f;


    //chunk ist wie normal ausgerichtet
    float distToTop = FVector::Dist(topRightRotated, playerInLocalSpace);
    float distToBottom = FVector::Dist(bottomRightRotated, playerInLocalSpace);
    topAxisLocked = distToTop > distToBottom; //weiter weg: lock axis
    bottomAxisLocked = !topAxisLocked;
    
    //spieler ist zum chunk seitlich ausgerichtet
    float distToLeft = FVector::Dist(topLeftRotated, playerInLocalSpace);
    float distToRight = FVector::Dist(topRightRotated, playerInLocalSpace);
    leftAxisLocked = distToLeft > distToRight; //weiter weg: lock axis
    rightAxisLocked = !leftAxisLocked;

    /*FString message = FString::Printf(
        TEXT("DEBUGLOCKAXIS top %d, bottom %d, left %d, right %d"),
        topAxisLocked ? 1 : 0,
        bottomAxisLocked ? 1 : 0,
        leftAxisLocked ? 1 : 0,
        rightAxisLocked ? 1 : 0
    );
    */
    //DebugHelper::logMessage(message);
}








FVector AcustomWaterActor::playerLocation(){
    referenceManager *manager = referenceManager::instance();
    if(manager != nullptr){
        return manager->playerLocation();
    }

    return FVector(0, 0, 0);
}

bool AcustomWaterActor::playerIsInBounds(){
    FVector location = playerLocation();
    return inBoundsOfPane(location);
}

bool AcustomWaterActor::inBoundsOfPane(FVector &vec){
    FVector ownOffset = GetActorLocation();
    FVector bl = BottomLeft + ownOffset;
    FVector tr = TopRight + ownOffset;

    return (vec.X >= bl.X && vec.X < tr.X) && (vec.Y >= bl.Y && vec.Y < tr.Y);
}




// Fill out your copyright notice in the Description page of Project Settings.


#include "customWaterActor.h"
#include "p2/gamestart/assetManager.h"
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
    if(doTick()){
        //updateRunningTime(DeltaTime);
        TickRipples(DeltaTime); //tick ripples before vertex shader to already modify mesh
        vertexShader();
        SetActorHiddenInGame(false);
    }else{
        SetActorHiddenInGame(true);
    }
    
}


void AcustomWaterActor::createWaterPane(
    UWorld *world, 
    int vertexCount
){
    if(world == nullptr){
        return;
    }

    int distanceBetweenVertecies = DEFAULT_DISTANCE_BETWEEN_VERTECIES;
    int paneCount = vertexCount / MAX_VERTEXCOUNT;
    int offsetOnAxis = MAX_VERTEXCOUNT * distanceBetweenVertecies;
    FVector offsetVector(-2000, -2000, 100);

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
                SpawnedActor->createWaterPane(MAX_VERTEXCOUNT, distanceBetweenVertecies);
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

    BottomLeft = FVector(0,0,0);
	TopLeft = FVector(0, vertexCountIn * detail, 0);
	BottomRight = FVector(vertexCountIn * detail,0,0);
	TopRight = FVector(vertexCountIn * detail,vertexCountIn * detail,0);


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


    //exclude this for boen controller raycast
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
                /*
                if(isInRangeForTick(vertex, locationOfPlayer)){
                    applyShaderToVertex(vertex);
                    applyWaterRippleOffset(vertex, actorLocation);
                }*/

                //is testet, skips vertecies if out of range based on direction
                
                bool wasTicked = false;
                int dirX = isInRangeForTickOnX(vertex, locationOfPlayer);
                if (dirX == 0)
                {
                    int dirY = isInRangeForTickOnY(vertex, locationOfPlayer);
                    if(dirY == 0){
                        applyShaderToVertex(vertex);
                        applyWaterRippleOffset(vertex, actorLocation);
                        wasTicked = true;
                    }
                    if(dirY < 0){
                        //nichts, i++
                    }
                    if(dirY > 0){
                        //goto next start of x
                        int column = i / vertexcountX;
                        i = (column + 1) * vertexcountX;
                    }
                }
                if(dirX < 0){
                    //skip to next column
                    i += vertexcountX;
                }

                if(!wasTicked){
                    vertex.Z = 0.0f;
                }

                if(dirX > 0){
                    //skip all
                    break;
                }

            }
            
        }

        refreshMesh(*thisMesh, waterMesh, layer);
    }
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


void AcustomWaterActor::addNewRipple(FVector &location){
    if(rippleVector.size() == 0 || rippleVecSize >= rippleVector.size()){
        rippleVector.push_back(ripple(location));
        rippleVecSize = rippleVector.size();
    }
    
    if(rippleVecSize < rippleVector.size()){
        int last = rippleVecSize;
        rippleVector[last].init(location); //reuse object
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
bool AcustomWaterActor::doTick(){
    return meshInited && playerIsInBounds() && TickBasedOnPlayerDistance();
}

bool AcustomWaterActor::TickBasedOnPlayerDistance(){
    referenceManager *manager = referenceManager::instance();
    if(manager != nullptr){
        FVector locationOfPlayer = playerLocation();

        ELod lodResult = lodLevelByDistanceTo(locationOfPlayer);
            
        //only lod near allowed
        if(lodResult != ELod::lodNear){
            return false;
        }
        return true;
    }
    return false;
}




bool AcustomWaterActor::isInRangeForTick(FVector &vertex, FVector &locationOfPlayer){
    FVector offset = GetActorLocation();
    int xdist = std::abs(offset.X + vertex.X - locationOfPlayer.X);
    if(xdist > MAX_DISTANCE){
        return false;
    }

    int ydist = std::abs(offset.Y + vertex.Y - locationOfPlayer.Y);
    if(ydist > MAX_DISTANCE){
        return false;
    }
    return true;
}


int AcustomWaterActor::isInRangeForTickOnX(
    FVector &vertex, 
    FVector &locationOfPlayer
){
    FVector vertexWorld = GetActorLocation();
    vertexWorld += vertex;
    int xdist = vertexWorld.X - locationOfPlayer.X;
    if (std::abs(xdist) > MAX_DISTANCE)
    {
        if(vertexWorld.X > locationOfPlayer.X){
            return 1;
        }
        return -1;
    }
    return 0;
}

int AcustomWaterActor::isInRangeForTickOnY(
    FVector &vertex, 
    FVector &locationOfPlayer
){
    FVector vertexWorld = GetActorLocation();
    vertexWorld += vertex;
    int ydist = vertexWorld.Y - locationOfPlayer.Y;
    if (std::abs(ydist) > MAX_DISTANCE)
    {
        if(vertexWorld.Y > locationOfPlayer.Y){
            return 1;
        }
        return -1;
    }
    return 0;
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




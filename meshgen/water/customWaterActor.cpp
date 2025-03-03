// Fill out your copyright notice in the Description page of Project Settings.


#include "customWaterActor.h"
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
    setTeam(teamEnum::neutralTeam);
}


void AcustomWaterActor::Tick(float DeltaTime){
    Super::Tick(DeltaTime);
    if(meshInited){
        if(TickBasedOnPlayerDistance()){
            updateRunningTime(DeltaTime);
            TickRipples(DeltaTime);
            vertexShader();
        }

        
    }
    
}

void AcustomWaterActor::updateRunningTime(float deltaTime){
    runningTime += deltaTime;
    if(runningTime > 2 * M_PI){
        runningTime = 0.0f;
    }
}



/// @brief 
/// @param sizeX in vertecies
/// @param sizeY in vertecies
/// @param detail detail between vertecies (in cm)
void AcustomWaterActor::createWaterPane(int vertexCountXIn, int vertexCountYIn, int detail){
    if(meshInited){
        return;
    }

    vertexcountX = std::abs(vertexCountXIn);
    vertexcountY = std::abs(vertexCountYIn);
    if(vertexcountX <= 2){
        vertexcountX = 3;
    }
    if(vertexcountY <= 2){
        vertexcountY = 3;
    }

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

    //raycast is not blocked by water
    UProceduralMeshComponent *thisMesh = meshComponentPointer();
    if(thisMesh){
        TArray<FVector> &vertecies = waterMesh.getVerteciesRef();

        FVector actorLocation = GetActorLocation();
        for (int i = 0; i < vertecies.Num(); i++)
        {
            FVector &vertex = vertecies[i];
            applyCurve(vertex);
            applyWaterRippleOffset(vertex, actorLocation);
        }

        refreshMesh(*thisMesh, waterMesh, layer);
    }
}


/// @brief apply vertex shader to the given vertex
/// @param vertex vertex to move
void AcustomWaterActor::applyCurve(FVector &vertex){
    FVector actorLocation = GetActorLocation();
    float distXAll = vertex.X + actorLocation.X;
    float distYAll = vertex.Y + actorLocation.Y;

    float frequency = 0.01f; // Wellenbreite
    float amplitude = 10.0f; // Wellenhöhe
    float speed = 1.0f; // Wellengeschwindigkeit //1.0;
    float wave = sin(distXAll * frequency + runningTime * speed) + cos(distYAll * frequency + runningTime * speed);

    vertex.Z = wave * amplitude;

}



void AcustomWaterActor::refreshMesh(
    UProceduralMeshComponent& meshComponent,
    MeshData &other,
    int layer
){
    if(meshInited){

        double startTime = FPlatformTime::Seconds();

        meshComponent.UpdateMeshSection(
            layer, 
            other.getVerteciesRef(), 
            other.getNormalsRef(), 
            other.getUV0Ref(),
            other.getVertexColorsRef(), 
            other.getTangentsRef()
        );

        // Berechne die verstrichene Zeit
        double endTime = FPlatformTime::Seconds();
        double deltaTime = endTime - startTime;

        if(false){
            DebugHelper::showScreenMessage("update time ", (float) deltaTime);
        }

        meshComponent.SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        meshComponent.SetCollisionResponseToAllChannels(ECR_Ignore);
        meshComponent.SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
        
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
bool AcustomWaterActor::TickBasedOnPlayerDistance(){
    referenceManager *manager = referenceManager::instance();
    if(manager != nullptr){
        FVector locationOfPlayer = manager->playerLocation();
        FVector ownLocation = GetActorLocation();

        FVector connect = ownLocation - locationOfPlayer;
        FVector playerLook = manager->playerLookDir();
        if(connect.X * playerLook.X + connect.Y * playerLook.Y > 0.0f){ //in blickrichtung auf 180 grad ebene
            ELod lodResult = lodLevelByDistanceTo(locationOfPlayer);
            
            //only lod near allowed
            if(lodResult == ELod::lodFar){
                return false;
            }
            return true;
        }
    }
    return false;
}
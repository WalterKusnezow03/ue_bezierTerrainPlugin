#include "CustomMeshUIActor.h" 

#include "Kismet/KismetMathLibrary.h"
 
ACustomMeshUIActor::ACustomMeshUIActor() 
{ 
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it. 
    PrimaryActorTick.bCanEverTick = true; 
    PrimaryActorTick.bTickEvenWhenPaused = true; 
 
    // Create the widget component as root
    Widget = CreateDefaultSubobject<UAnyMeshWidgetComponent>(TEXT("Widget")); 
    RootComponent = Widget;
    
    // Set scale to 0.5 on all axes 
    Widget->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f)); 
 
    Widget->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); 
    Widget->SetCollisionResponseToAllChannels(ECR_Ignore); 
    Widget->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block); 
    Widget->SetCollisionResponseToChannel(ECC_Camera, ECR_Block); 
    Widget->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block); 
 
    Widget->SetTwoSided(true); 
    Widget->SetBlendMode(EWidgetBlendMode::Transparent); 
     
    Widget->SetDrawAtDesiredSize(true); 
    Widget->SetGeometryMode(EWidgetGeometryMode::Plane); //custom meshdata on plane mode.
    //EWidgetGeometryMode::Cylinder
    //Widget->SetCylinderArcAngle(30.0f);

	
} 
 
// Called when the game starts or when spawned 
void ACustomMeshUIActor::BeginPlay() 
{ 
    Super::BeginPlay();
    SetWidgetClassOnBeginPlay();
    CreateWidgetMeshData();
}

void ACustomMeshUIActor::SetWidgetClassOnBeginPlay(){
    SetWidgetClass(CurrentWidgetClass);
}

void ACustomMeshUIActor::CreateWidgetMeshData(){
    if(!bMeshDataCreated && Widget){
        FVector v0(0, 0, 0);
        FVector v1(0, 0, 100);
        FVector v2(0, 200, 100);
        FVector v3(0, 200, 50);

        MeshData &data = Widget->GetMeshDataRef();
        data.append(v0,v1,v2);
        data.append(v0,v2,v3);

        FVector2D uv0(0, 0);
        FVector2D uv1(0, 1);
        FVector2D uv2(1, 1);
        FVector2D uv3(1, 0);
        data.appendUvs(uv0, uv1, uv2);
        data.appendUvs(uv0, uv2, uv3);
    

        data.calculateNormals();
        bMeshDataCreated = true;
    }
}

void ACustomMeshUIActor::SetDrawSize(FVector2D size){
    if(Widget){
        Widget->SetDrawSize(size);
    }
    
}

// Called every frame 
void ACustomMeshUIActor::Tick(float DeltaTime) 
{ 
    Super::Tick(DeltaTime); 
    
} 
 
void ACustomMeshUIActor::SetWidgetClass(TSubclassOf<UUserWidget> NewWidgetClass) 
{ 
    if (NewWidgetClass) 
    {
       CurrentWidgetClass = NewWidgetClass; 
       Widget->SetWidgetClass(NewWidgetClass);
    } 
} 
 
TSubclassOf<UUserWidget> ACustomMeshUIActor::GetWidgetClass() const 
{ 
    return CurrentWidgetClass;
}
#pragma once

#include "Components/WidgetComponent.h"

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Templates/SubclassOf.h"
#include "Layout/ArrangedWidget.h"
#include "WorldCollision.h"
#include "Components/MeshComponent.h"
#include "Blueprint/UserWidget.h"

#include "Engine/GameInstance.h"
#include "Materials/Material.h"
#include "PrimitiveViewRelevance.h"
#include "PrimitiveSceneProxy.h"
#include "UObject/ConstructorHelpers.h"
#include "EngineGlobals.h"
#include "MaterialShared.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialRenderProxy.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Widgets/SWindow.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Framework/Application/SlateApplication.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Input/HittestGrid.h"
#include "SceneManagement.h"
#include "DynamicMeshBuilder.h"
#include "PhysicsEngine/BoxElem.h"
#include "PhysicsEngine/BodySetup.h"
#include "Slate/SGameLayerManager.h"
#include "Slate/WidgetRenderer.h"
#include "Slate/SWorldWidgetScreenLayer.h"
#include "UObject/EditorObjectVersion.h"
#include "Widgets/SViewport.h"
#include "SceneInterface.h"

//custom mesh data
#include "GameCore/MeshGenBase/MeshData/MeshData.h"


//#include "GameCore/MeshGenBase/WidgetComponentModified/CopiedSource/WidgetComponentCustom.h"

#include "AnyMeshWidgetComponent.generated.h"


UCLASS(Blueprintable, ClassGroup="UserInterface", hidecategories=(Object,Activation,"Components|Activation",Sockets,Base,Lighting,LOD,Mesh), editinlinenew, meta=(BlueprintSpawnableComponent) )
class GAMECORE_API UAnyMeshWidgetComponent : public UWidgetComponent
//UWidgetComponentCustom
{
	GENERATED_BODY()
public:
    UAnyMeshWidgetComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
    {
        SetGeometryMode(EWidgetGeometryMode::Plane); //to force custom meshdata
    }

    virtual void BeginPlay() override;
    virtual void TickComponent(
        float DeltaTime,
        ELevelTick TickType,
        FActorComponentTickFunction *ThisTickFunction) override;

    //wannnn wird das ausgerufen
    virtual FPrimitiveSceneProxy* CreateSceneProxy() override;

    /** Ensures the 3d window is created its size and content. */
	virtual void UpdateWidget() override {
        Super::UpdateWidget();
    }

    void OverrideMeshData(MeshData &data);

    MeshData &GetMeshDataRef();

protected:
    MeshData assignedMeshData;
    bool MeshDataWasModified = false;

    void FlagMeshDataDirty();

    void CreateMaterial();
};
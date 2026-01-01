#include "AnyMeshWidgetComponent.h"

#include "DynamicMeshWidgetSceneProxy.h"
/*
UAnyMeshWidgetComponent::UAnyMeshWidgetComponent(const FObjectInitializer &ObjectInitializer)
    : Super(ObjectInitializer) {

      };*/

void UAnyMeshWidgetComponent::BeginPlay(){
	Super::BeginPlay();

	DebugHelper::logMessage("UAnyMeshWidgetComponent::BeginPlay");
	if(assetManager *a = assetManager::instance()){
		DebugHelper::logMessage("UAnyMeshWidgetComponent::TryFindMaterial");
		UMaterialInterface *BaseMat = a->Find<materialEnum, UMaterial>(materialEnum::widgetMaterial);
		if (BaseMat){
			DebugHelper::logMessage("UAnyMeshWidgetComponent::TryFindMaterial Success");
			MaterialInstance = UMaterialInstanceDynamic::Create(BaseMat, this);
		}
	}
}

void UAnyMeshWidgetComponent::TickComponent(
	float DeltaTime,
	ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction
){
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(MeshDataWasModified){
		MarkRenderStateDirty(); //CreateSceneProxy will be called again.
	}
}

void UAnyMeshWidgetComponent::FlagMeshDataDirty(){
	MeshDataWasModified = true;
}


void UAnyMeshWidgetComponent::OverrideMeshData(MeshData &data){
	assignedMeshData = data;
	//MarkRenderStateDirty(); //calls CreateSceneProxy?
	FlagMeshDataDirty();
}

MeshData &UAnyMeshWidgetComponent::GetMeshDataRef(){
	FlagMeshDataDirty();
	return assignedMeshData;
}

/// ------------  TO BE CHANGED ------------

FPrimitiveSceneProxy* UAnyMeshWidgetComponent::CreateSceneProxy()
{
	if (Space == EWidgetSpace::Screen)
	{
		return nullptr;
	}

    ///CurrentSlateWidget is private inside the
    //widget component
    //but we can say that only UUserWidgets are allowed
    //with:
	//DebugHelper::showScreenMessage("UAnyMeshWidgetComponent::CreateProxy Called");

	CreateMaterial();

	//if (WidgetRenderer && CurrentSlateWidget.IsValid()) //private member var. 
    if(WidgetRenderer && GetWidget() && GetMaterialInstance())
	{
		//is called every frame.
		//DebugHelper::showScreenMessage("UAnyMeshWidgetComponent::CreateProxy Valid Called");

		RequestRenderUpdate();
		LastWidgetRenderTime = 0;

		return new FDynamicMeshWidgetSceneProxy(
			this, 
			*WidgetRenderer->GetSlateRenderer(),
			assignedMeshData //copy overhead (?)
		);
	}
    return Super::CreateSceneProxy();
}

void UAnyMeshWidgetComponent::CreateMaterial(){
	if(!GetMaterialInstance()){

		UMaterialInterface *BaseMat = nullptr;
		if(assetManager *a = assetManager::instance()){
			DebugHelper::logMessage("UAnyMeshWidgetComponent::TryFindMaterial");
			BaseMat = a->Find<materialEnum, UMaterial>(materialEnum::widgetMaterial);
		}
		if(!BaseMat){
			BaseMat = UMaterial::GetDefaultMaterial(MD_Surface);
		}
		if (BaseMat){
			MaterialInstance = UMaterialInstanceDynamic::Create(BaseMat, this);
		}
	}
}
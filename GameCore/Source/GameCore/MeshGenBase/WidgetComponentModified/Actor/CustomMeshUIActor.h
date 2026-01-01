#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "GameCore/MeshGenBase/WidgetComponentModified/Component/AnyMeshWidgetComponent.h"

#include "Blueprint/UserWidget.h"
#include "Components/SceneComponent.h"
#include "GameFramework/PlayerController.h"
#include "CustomMeshUIActor.generated.h"

/**
 * UI Actor that contains a 3D Widget Component with dynamic class changing and generic getter
 */
UCLASS()
class GAMECORE_API ACustomMeshUIActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACustomMeshUIActor();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void SetWidgetClassOnBeginPlay();

	//create your meshdata here, as expected default, no winding order flipping. 
	//winding order will be flipped inside scene proxy, but is
	//needed because widgets are otherwise black.
	virtual void CreateWidgetMeshData();
	bool bMeshDataCreated = false;

public:
	void SetDrawSize(FVector2D size);

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// 3D Widget Component that displays UI in the world space
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	UAnyMeshWidgetComponent* Widget;
	
	// Set a new widget class for the widget component
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetWidgetClass(TSubclassOf<UUserWidget> NewWidgetClass);

	// Get the current widget class
	UFUNCTION(BlueprintPure, Category = "UI")
	TSubclassOf<UUserWidget> GetWidgetClass() const;

	// Generic template method to get the actual widget inside the widget component
	template<typename T>
	T* GetWidget() const
	{
		if (Widget && Widget->GetUserWidgetObject())
		{
			return Cast<T>(Widget->GetUserWidgetObject());
		}
		return nullptr;
	}

protected:
	// Current widget class reference - can be edited in the editor and will update the widget component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> CurrentWidgetClass;
};
#pragma once

/// --- modified source code to support custom meshdata! ---

#include "AnyMeshWidgetComponent.h"

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

/** Represents a billboard sprite to the scene manager. */
class GAMECORE_API FDynamicMeshWidgetSceneProxy : public FPrimitiveSceneProxy
{
public:

	//lol???
	bool AllowDebugViewmodes() const {
		return true;
	}

protected:
	MeshData internalMeshData;

	void BuildCustomMeshData(
		const TArray<const FSceneView *> &Views,
		const FMatrix &ViewportLocalToWorld,
		FMatrix &PreviousLocalToWorld,
		FMaterialRenderProxy *ParentMaterialProxy,
		FMeshElementCollector &Collector,
		uint32 VisibilityMap
	) const;
	void BuildCustomMeshData(
		FDynamicMeshBuilder &MeshBuilder
	) const;
	FVector3f As3f(const FVector &vertex) const{
		return FVector3f(vertex.X, vertex.Y, vertex.Z);
	}

	FVector2f As2f(const FVector2D &vertex) const{
		return FVector2f(vertex.X, vertex.Y);
	}

	void AddTriangle(
		FDynamicMeshBuilder &MeshBuilder,
		const FVector &v0,
		const FVector &v1,
		const FVector &v2,
		const FVector &normal0,
		const FVector &normal1,
		const FVector &normal2,
		const FVector2D uv0,
		const FVector2D uv1,
		const FVector2D uv2
	) const;

public:
	SIZE_T GetTypeHash() const override
	{
		static size_t UniquePointer;
		return reinterpret_cast<size_t>(&UniquePointer);
	}

	/** Initialization constructor. */
	FDynamicMeshWidgetSceneProxy(
		UAnyMeshWidgetComponent *InComponent, 
		ISlate3DRenderer& InRenderer,
		MeshData &meshData
	)
		: FPrimitiveSceneProxy( InComponent )
		, ArcAngle(FMath::DegreesToRadians(InComponent->GetCylinderArcAngle()))
		, Pivot( InComponent->GetPivot() )
		, Renderer( InRenderer )
		, RenderTarget( InComponent->GetRenderTarget() )
		, MaterialInstance( InComponent->GetMaterialInstance() )
		, BlendMode( InComponent->GetBlendMode() )
		, GeometryMode(InComponent->GetGeometryMode())
		, BodySetup(InComponent->GetBodySetup())
	{
		bWillEverBeLit = false;
		internalMeshData = meshData;
		
		//DO NOT REMOVE
		//Fix broken triangles,
		//text is black if winding order is not reversed
		//DO NOT REMOVE
		internalMeshData.flipWindingOrder();
		
		if(MaterialInstance){
			if(false){
				DebugHelper::logMessage(
					FString::Printf(
						TEXT("FDynamicMeshWidgetSceneProxy::MaterialInstanceName: %s"),
						*MaterialInstance->GetPathName()
					)
				);
			}
			
		}
		if(MaterialInstance){
			MaterialRelevance = MaterialInstance->GetRelevance_Concurrent(GetScene().GetFeatureLevel());

			//remove depth? (doesnt help)
			//MaterialRelevance.bDisableDepthTest = true;
		}
		
		
	}

	
	// FPrimitiveSceneProxy interface.
	virtual void GetDynamicMeshElements(
		const TArray<const FSceneView*>& Views, 
		const FSceneViewFamily& ViewFamily, 
		uint32 VisibilityMap, 
		FMeshElementCollector& Collector
	) const override;

	

	void RenderCollision(UBodySetup* InBodySetup, FMeshElementCollector& Collector, int32 ViewIndex, const FEngineShowFlags& EngineShowFlags, const FBoxSphereBounds& InBounds, bool bRenderInEditor) const
	{
		if ( InBodySetup )
		{
			bool bDrawCollision = EngineShowFlags.Collision && IsCollisionEnabled();

			#if WITH_EDITOR
			if ( bDrawCollision && AllowDebugViewmodes() )
			{
				// Draw simple collision as wireframe if 'show collision', collision is enabled, and we are not using the complex as the simple
				const bool bDrawSimpleWireframeCollision = InBodySetup->CollisionTraceFlag != ECollisionTraceFlag::CTF_UseComplexAsSimple;

				if ( FMath::Abs(GetLocalToWorld().Determinant()) < SMALL_NUMBER )
				{
					// Catch this here or otherwise GeomTransform below will assert
					// This spams so commented out
					//UE_LOG(LogStaticMesh, Log, TEXT("Zero scaling not supported (%s)"), *StaticMesh->GetPathName());
				}
				else
				{
					const bool bDrawSolid = !bDrawSimpleWireframeCollision;
					const bool bProxyIsSelected = IsSelected();

					if ( bDrawSolid )
					{
						// Make a material for drawing solid collision stuff
						auto SolidMaterialInstance = new FColoredMaterialRenderProxy(
							GEngine->ShadedLevelColorationUnlitMaterial->GetRenderProxy(),
							GetWireframeColor()
							);

						Collector.RegisterOneFrameMaterialProxy(SolidMaterialInstance);

						FTransform GeomTransform(GetLocalToWorld());
						InBodySetup->AggGeom.GetAggGeom(GeomTransform, GetWireframeColor().ToFColor(true), SolidMaterialInstance, false, true, AlwaysHasVelocity(), ViewIndex, Collector);
					}
					// wireframe
					else
					{
						FColor CollisionColor = FColor(157, 149, 223, 255);
						FTransform GeomTransform(GetLocalToWorld());
						InBodySetup->AggGeom.GetAggGeom(GeomTransform, GetSelectionColor(CollisionColor, bProxyIsSelected, IsHovered()).ToFColor(true), nullptr, false, false, AlwaysHasVelocity(), ViewIndex, Collector);
					}
				}
			}
			#endif
		}
	}

	virtual FPrimitiveViewRelevance GetViewRelevance(const FSceneView* View) const override
	{
		bool bVisible = true;

		FPrimitiveViewRelevance Result;

		MaterialRelevance.SetPrimitiveViewRelevance(Result);

		Result.bDrawRelevance = IsShown(View) && bVisible && View->Family->EngineShowFlags.WidgetComponents;
		Result.bDynamicRelevance = true;
		Result.bRenderCustomDepth = ShouldRenderCustomDepth();
		Result.bRenderInMainPass = ShouldRenderInMainPass();
		Result.bUsesLightingChannels = GetLightingChannelMask() != GetDefaultLightingChannelMask();
		Result.bShadowRelevance = IsShadowCast(View);
		Result.bTranslucentSelfShadow = bCastVolumetricTranslucentShadow;
		Result.bEditorPrimitiveRelevance = false;
		Result.bVelocityRelevance = DrawsVelocity() && Result.bOpaque && Result.bRenderInMainPass;

		return Result;
	}

	virtual void GetLightRelevance(const FLightSceneProxy* LightSceneProxy, bool& bDynamic, bool& bRelevant, bool& bLightMapped, bool& bShadowMapped) const override
	{
		bDynamic = false;
		bRelevant = false;
		bLightMapped = false;
		bShadowMapped = false;
	}

	virtual void OnTransformChanged() override
	{
		Origin = GetLocalToWorld().GetOrigin();
	}

	virtual bool CanBeOccluded() const override
	{
		return !MaterialRelevance.bDisableDepthTest;
	}

	virtual uint32 GetMemoryFootprint(void) const override { return(sizeof(*this) + GetAllocatedSize()); }

	uint32 GetAllocatedSize(void) const { return( FPrimitiveSceneProxy::GetAllocatedSize() ); }

private:
	FVector Origin;
	float ArcAngle;
	FVector2D Pivot;
	ISlate3DRenderer& Renderer;
	UTextureRenderTarget2D* RenderTarget;
	UMaterialInstanceDynamic* MaterialInstance;
	FMaterialRelevance MaterialRelevance;
	EWidgetBlendMode BlendMode;
	EWidgetGeometryMode GeometryMode;
	UBodySetup* BodySetup;




	template <typename T>
	bool IndexInBound(const TArray<T> &array, const int32 index) const {
		return index >= 0 && index < array.Num();
	}

	template <typename T>
	bool IndexInBound(
		const TArray<T> &array, 
		const int32 i0, 
		const int32 i1, 
		const int32 i2
	) const {
		return 
		IndexInBound(array, i0) &&
		IndexInBound(array, i1) &&
		IndexInBound(array, i2);
	}

	void FindTangents(
		const FVector &v0,
		const FVector &v1,
		const FVector &v2,
		FVector &tangentX,
		FVector &tangentY
	) const;
};

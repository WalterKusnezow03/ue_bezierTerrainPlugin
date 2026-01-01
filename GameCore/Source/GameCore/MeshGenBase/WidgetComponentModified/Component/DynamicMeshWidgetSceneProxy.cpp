#include "DynamicMeshWidgetSceneProxy.h"

#include "DebugPlugin/DebugHelper.h"



// FPrimitiveSceneProxy interface.
void FDynamicMeshWidgetSceneProxy::GetDynamicMeshElements(
    const TArray<const FSceneView*>& Views, 
    const FSceneViewFamily& ViewFamily, 
    uint32 VisibilityMap, 
    FMeshElementCollector& Collector
) const 
{
    if(!MaterialInstance){
        DebugHelper::logMessage("FDynamicMeshWidgetSceneProxy::Cannot create mesh, no material");
        return;
    }

#if WITH_EDITOR
    const bool bWireframe = AllowDebugViewmodes() && ViewFamily.EngineShowFlags.Wireframe;

    auto WireframeMaterialInstance = new FColoredMaterialRenderProxy(
        GEngine->WireframeMaterial ? GEngine->WireframeMaterial->GetRenderProxy() : nullptr,
        FLinearColor(0, 0.5f, 1.f)
        );

    Collector.RegisterOneFrameMaterialProxy(WireframeMaterialInstance);

    FMaterialRenderProxy* ParentMaterialProxy = nullptr;
    if ( bWireframe )
    {
        ParentMaterialProxy = WireframeMaterialInstance;
    }
    else
    {
        ParentMaterialProxy = MaterialInstance->GetRenderProxy();
    }
#else
    FMaterialRenderProxy* ParentMaterialProxy = MaterialInstance->GetRenderProxy();
#endif

    //FSpriteTextureOverrideRenderProxy* TextureOverrideMaterialProxy = new FSpriteTextureOverrideRenderProxy(ParentMaterialProxy,

    const FMatrix& ViewportLocalToWorld = GetLocalToWorld();

    FMatrix PreviousLocalToWorld;

    if (!GetScene().GetPreviousLocalToWorld(GetPrimitiveSceneInfo(), PreviousLocalToWorld))
    {
        PreviousLocalToWorld = GetLocalToWorld();
    }

    if( RenderTarget )
    {
        FTextureResource* TextureResource = RenderTarget->GetResource();
        if ( TextureResource )
        {
            if (GeometryMode == EWidgetGeometryMode::Plane)
            {
                DebugHelper::showScreenMessage("FDynamicMeshWidgetSceneProxy::BuildMeshData");
                BuildCustomMeshData(
                    Views,
                    ViewportLocalToWorld,
                    PreviousLocalToWorld,
                    ParentMaterialProxy,
                    Collector,
                    VisibilityMap);
            }
            else
            {
                ensure(GeometryMode == EWidgetGeometryMode::Cylinder);

                const int32 NumSegments = FMath::Lerp(4, 32, ArcAngle/PI);


                const float Radius = RenderTarget->SizeX / ArcAngle;
                const float Apothem = Radius * FMath::Cos(0.5f*ArcAngle);
                const float ChordLength = 2.0f * Radius * FMath::Sin(0.5f*ArcAngle);
                
                const float PivotOffsetX = ChordLength * (0.5-Pivot.X);
                const float V = -RenderTarget->SizeY * Pivot.Y;
                const float VL = RenderTarget->SizeY * (1.0f - Pivot.Y);

                int32 VertexIndices[4];

                for (int32 ViewIndex = 0; ViewIndex < Views.Num(); ViewIndex++)
                {
                    FDynamicMeshBuilder MeshBuilder(Views[ViewIndex]->GetFeatureLevel());

                    if (VisibilityMap & (1 << ViewIndex))
                    {
                        const float RadiansPerStep = ArcAngle / NumSegments;

                        FVector LastTangentX;
                        FVector LastTangentY;
                        FVector LastTangentZ;

                        for (int32 Segment = 0; Segment < NumSegments; Segment++ )
                        {
                            const float Angle = -ArcAngle / 2 + Segment * RadiansPerStep;
                            const float NextAngle = Angle + RadiansPerStep;
                            
                            // Polar to Cartesian
                            const float X0 = Radius * FMath::Cos(Angle) - Apothem;
                            const float Y0 = Radius * FMath::Sin(Angle);
                            const float X1 = Radius * FMath::Cos(NextAngle) - Apothem;
                            const float Y1 = Radius * FMath::Sin(NextAngle);

                            const float U0 = static_cast<float>(Segment) / NumSegments;
                            const float U1 = static_cast<float>(Segment+1) / NumSegments;

                            const FVector Vertex0 = -FVector(X0, PivotOffsetX + Y0, V);
                            const FVector Vertex1 = -FVector(X0, PivotOffsetX + Y0, VL);
                            const FVector Vertex2 = -FVector(X1, PivotOffsetX + Y1, VL);
                            const FVector Vertex3 = -FVector(X1, PivotOffsetX + Y1, V);

                            FVector TangentX = Vertex3 - Vertex0;
                            TangentX.Normalize();
                            FVector TangentY = Vertex1 - Vertex0;
                            TangentY.Normalize();
                            FVector TangentZ = FVector::CrossProduct(TangentX, TangentY);

                            if (Segment == 0)
                            {
                                LastTangentX = TangentX;
                                LastTangentY = TangentY;
                                LastTangentZ = TangentZ;
                            }

                            VertexIndices[0] = MeshBuilder.AddVertex((FVector3f)Vertex0, FVector2f(U0, 0), (FVector3f)LastTangentX, (FVector3f)LastTangentY, (FVector3f)LastTangentZ, FColor::White);
                            VertexIndices[1] = MeshBuilder.AddVertex((FVector3f)Vertex1, FVector2f(U0, 1), (FVector3f)LastTangentX, (FVector3f)LastTangentY, (FVector3f)LastTangentZ, FColor::White);
                            VertexIndices[2] = MeshBuilder.AddVertex((FVector3f)Vertex2, FVector2f(U1, 1), (FVector3f)TangentX, (FVector3f)TangentY, (FVector3f)TangentZ, FColor::White);
                            VertexIndices[3] = MeshBuilder.AddVertex((FVector3f)Vertex3, FVector2f(U1, 0), (FVector3f)TangentX, (FVector3f)TangentY, (FVector3f)TangentZ, FColor::White);

                            MeshBuilder.AddTriangle(VertexIndices[0], VertexIndices[1], VertexIndices[2]);
                            MeshBuilder.AddTriangle(VertexIndices[0], VertexIndices[2], VertexIndices[3]);

                            LastTangentX = TangentX;
                            LastTangentY = TangentY;
                            LastTangentZ = TangentZ;
                        }

                        FDynamicMeshBuilderSettings Settings;
                        Settings.bDisableBackfaceCulling = false;
                        Settings.bReceivesDecals = true;
                        Settings.bUseSelectionOutline = true;
                        MeshBuilder.GetMesh(ViewportLocalToWorld, PreviousLocalToWorld, ParentMaterialProxy, SDPG_World, Settings, nullptr, ViewIndex, Collector, FHitProxyId());
                    }
                }
            }
        }
    }

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
    for ( int32 ViewIndex = 0; ViewIndex < Views.Num(); ViewIndex++ )
    {
        if ( VisibilityMap & ( 1 << ViewIndex ) )
        {
            RenderCollision(BodySetup, Collector, ViewIndex, ViewFamily.EngineShowFlags, GetBounds(), IsSelected());
            RenderBounds(Collector.GetPDI(ViewIndex), ViewFamily.EngineShowFlags, GetBounds(), IsSelected());
        }
    }
#endif
}






void FDynamicMeshWidgetSceneProxy::BuildCustomMeshData(
    const TArray<const FSceneView*>& Views,
    const FMatrix &ViewportLocalToWorld,
    FMatrix &PreviousLocalToWorld,
    FMaterialRenderProxy* ParentMaterialProxy,
    FMeshElementCollector& Collector,
    uint32 VisibilityMap
)const{
    
    for ( int32 ViewIndex = 0; ViewIndex < Views.Num(); ViewIndex++ )
    {
        FDynamicMeshBuilder MeshBuilder(Views[ViewIndex]->GetFeatureLevel());

        if ( VisibilityMap & ( 1 << ViewIndex ) )
        {
            BuildCustomMeshData(MeshBuilder);

            FDynamicMeshBuilderSettings Settings;
            Settings.bDisableBackfaceCulling = false;
            Settings.bReceivesDecals = true;
            Settings.bUseSelectionOutline = true;
            MeshBuilder.GetMesh(
                ViewportLocalToWorld, 
                PreviousLocalToWorld, 
                ParentMaterialProxy, 
                SDPG_World, 
                Settings, 
                nullptr, 
                ViewIndex, 
                Collector, 
                FHitProxyId()
            );
        }
    }
}


void FDynamicMeshWidgetSceneProxy::BuildCustomMeshData(
    FDynamicMeshBuilder &MeshBuilder
)const{

    


    
    const TArray<int32> &triangles = internalMeshData.getTrianglesRefConst();
    const TArray<FVector> &vertecies = internalMeshData.getVerteciesRefConst();
    const TArray<FVector2D> &uvs = internalMeshData.getUV0RefConst();
    const TArray<FVector> &normals = internalMeshData.getNormalsRefConst();

    for (int32 t = 2; t < triangles.Num(); t += 3){
        const int32 t0 = triangles[t - 2];
        const int32 t1 = triangles[t - 1];
        const int32 t2 = triangles[t];

        if(
            IndexInBound(vertecies, t0, t1, t2) &&
            IndexInBound(normals, t0, t1, t2) &&
            IndexInBound(uvs, t0, t1, t2)
        ){
            const FVector &v0 = vertecies[t0];
            const FVector &v1 = vertecies[t1];
            const FVector &v2 = vertecies[t2];

            const FVector &n0 = normals[t0];
            const FVector &n1 = normals[t1];
            const FVector &n2 = normals[t2];

            const FVector2D &uv0 = uvs[t0];
            const FVector2D &uv1 = uvs[t1];
            const FVector2D &uv2 = uvs[t2];
            

            AddTriangle(
                MeshBuilder,
                v0,
                v1,
                v2,
                n0,
                n1,
                n2,
                uv0,
                uv1,
                uv2
            );
        }else{
            DebugHelper::logMessage("FDynamicMeshWidgetSceneProxy::MeshDataInvalid!");
        }
    }

        

}

void FDynamicMeshWidgetSceneProxy::AddTriangle(
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
) const {

    FVector tangentX3D;
    FVector tangentY3D;
    FindTangents(v0, v1, v2, tangentX3D, tangentY3D);

    FVector3f tangentX3f = As3f(tangentX3D);
    FVector3f tangentY3f = As3f(tangentY3D);

    
    int32 VertexIndices[3];
    VertexIndices[0] = MeshBuilder.AddVertex(
        As3f(v0), //vertex Pos
        As2f(uv0),  //UV
        tangentX3f, //FVector3f(0, -1, 0), //tangetX
        tangentY3f, //FVector3f(0, 0, -1), //tangetY
        As3f(normal0), //normal
        FColor::White
    );
    VertexIndices[1] = MeshBuilder.AddVertex(
        As3f(v1),
        As2f(uv1),  //UV
        tangentX3f, //FVector3f(0, -1, 0), //tangetX
        tangentY3f, //FVector3f(0, 0, -1), //tangetY
        As3f(normal1), 
        FColor::White
    );
    VertexIndices[2] = MeshBuilder.AddVertex(
        As3f(v2),
        As2f(uv2),  //UV
        tangentX3f, //FVector3f(0, -1, 0), //tangetX
        tangentY3f, //FVector3f(0, 0, -1), //tangetY
        As3f(normal2), 
        FColor::White
    );
    MeshBuilder.AddTriangle(VertexIndices[0], VertexIndices[1], VertexIndices[2]);
}


void FDynamicMeshWidgetSceneProxy::FindTangents(
    const FVector &v0,
    const FVector &v1,
    const FVector &v2,
    FVector &tangentX,
    FVector &tangentY
)const {
    FVector Edge1 = v1 - v0;
    FVector Edge2 = v2 - v0;

    tangentX = Edge1.GetSafeNormal();
    FVector Normal = FVector::CrossProduct(Edge1, Edge2).GetSafeNormal();
    tangentY = FVector::CrossProduct(Normal, tangentX);
}


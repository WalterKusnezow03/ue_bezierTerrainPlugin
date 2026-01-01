#include "ProceduralMeshComponentCustom.h"
#include "DebugPlugin/DebugHelper.h"














/// ---- deprecated not nesecarraly needed ----

void UProceduralMeshComponentCustom::UpdateMesh(int32 SectionIndex, MeshData &dataIn, bool bCreateCollision){
    
    
}

/*


void UProceduralMeshComponent::UpdateMeshSection(int32 SectionIndex, const TArray<FVector>& Vertices, const TArray<FVector>& Normals, const TArray<FVector2D>& UV0, const TArray<FVector2D>& UV1, const TArray<FVector2D>& UV2, const TArray<FVector2D>& UV3, const TArray<FColor>& VertexColors, const TArray<FProcMeshTangent>& Tangents)
{
	SCOPE_CYCLE_COUNTER(STAT_ProcMesh_UpdateSectionGT);

	if(SectionIndex < ProcMeshSections.Num())
	{
		FProcMeshSection& Section = ProcMeshSections[SectionIndex];
		const int32 NumVerts = Vertices.Num();
		const int32 PreviousNumVerts = Section.ProcVertexBuffer.Num();

		// See if positions are changing
		const bool bSameVertexCount = PreviousNumVerts == NumVerts;

		// Update bounds, if we are getting new position data
		if (bSameVertexCount)
		{
			Section.SectionLocalBox = FBox(Vertices);

			// Iterate through vertex data, copying in new info
			for (int32 VertIdx = 0; VertIdx < NumVerts; VertIdx++)
			{
				FProcMeshVertex& ModifyVert = Section.ProcVertexBuffer[VertIdx];

				// Position data
				if (Vertices.Num() == NumVerts)
				{
					ModifyVert.Position = Vertices[VertIdx];
				}

				// Normal data
				if (Normals.Num() == NumVerts)
				{
					ModifyVert.Normal = Normals[VertIdx];
				}

				// Tangent data
				if (Tangents.Num() == NumVerts)
				{
					ModifyVert.Tangent = Tangents[VertIdx];
				}

				// UV0 data
				if (UV0.Num() == NumVerts)
				{
					ModifyVert.UV0 = UV0[VertIdx];
				}
				// UV1 data
				if (UV1.Num() == NumVerts)
				{
					ModifyVert.UV1 = UV1[VertIdx];
				}
				// UV2 data
				if (UV2.Num() == NumVerts)
				{
					ModifyVert.UV2 = UV2[VertIdx];
				}
				// UV3 data
				if (UV3.Num() == NumVerts)
				{
					ModifyVert.UV3 = UV3[VertIdx];
				}

				// Color data
				if (VertexColors.Num() == NumVerts)
				{
					ModifyVert.Color = VertexColors[VertIdx];
				}
			}

			// If we have collision enabled on this section, update that too
			if (Section.bEnableCollision)
			{
				TArray<FVector> CollisionPositions;

				// We have one collision mesh for all sections, so need to build array of _all_ positions
				for (const FProcMeshSection& CollisionSection : ProcMeshSections)
				{
					// If section has collision, copy it
					if (CollisionSection.bEnableCollision)
					{
						for (int32 VertIdx = 0; VertIdx < CollisionSection.ProcVertexBuffer.Num(); VertIdx++)
						{
							CollisionPositions.Add(CollisionSection.ProcVertexBuffer[VertIdx].Position);
						}
					}
				}

				// Pass new positions to trimesh
				BodyInstance.UpdateTriMeshVertices(CollisionPositions);
			}

			// If we have a valid proxy and it is not pending recreation
			if (SceneProxy && !IsRenderStateDirty())
			{
				// Create data to update section
				FProcMeshSectionUpdateData* SectionData = new FProcMeshSectionUpdateData;
				SectionData->TargetSection = SectionIndex;
				SectionData->NewVertexBuffer = Section.ProcVertexBuffer;

				// Enqueue command to send to render thread
				FProceduralMeshSceneProxy* ProcMeshSceneProxy = (FProceduralMeshSceneProxy*)SceneProxy;
				ENQUEUE_RENDER_COMMAND(FProcMeshSectionUpdate)
				([ProcMeshSceneProxy, SectionData](FRHICommandListImmediate& RHICmdList) { ProcMeshSceneProxy->UpdateSection_RenderThread(SectionData); });
			}

			UpdateLocalBounds();		 // Update overall bounds
			MarkRenderTransformDirty();  // Need to send new bounds to render thread
		}
		else
		{
			UE_LOG(LogProceduralComponent, Error, TEXT("Trying to update a procedural mesh component section with a different number of vertices [Previous: %i, New: %i] (clear and recreate mesh section instead)"), PreviousNumVerts, NumVerts);
		}
	}
}

void UProceduralMeshComponent::ClearMeshSection(int32 SectionIndex)
{
	if (SectionIndex < ProcMeshSections.Num())
	{
		ProcMeshSections[SectionIndex].Reset();
		UpdateLocalBounds();
		UpdateCollision();
		MarkRenderStateDirty();
	}
}

void UProceduralMeshComponent::ClearAllMeshSections()
{
	ProcMeshSections.Empty();
	UpdateLocalBounds();
	UpdateCollision();
	MarkRenderStateDirty();
}

void UProceduralMeshComponent::SetMeshSectionVisible(int32 SectionIndex, bool bNewVisibility)
{
	if(SectionIndex < ProcMeshSections.Num())
	{
		// Set game thread state
		ProcMeshSections[SectionIndex].bSectionVisible = bNewVisibility;

		if (SceneProxy)
		{
			// Enqueue command to modify render thread info
			FProceduralMeshSceneProxy* ProcMeshSceneProxy = (FProceduralMeshSceneProxy*)SceneProxy;
			ENQUEUE_RENDER_COMMAND(FProcMeshSectionVisibilityUpdate)(
				[ProcMeshSceneProxy, SectionIndex, bNewVisibility](FRHICommandListImmediate& RHICmdList)
				{
					ProcMeshSceneProxy->SetSectionVisibility_RenderThread(SectionIndex, bNewVisibility);
				});
		}
	}
}

bool UProceduralMeshComponent::IsMeshSectionVisible(int32 SectionIndex) const
{
	return (SectionIndex < ProcMeshSections.Num()) ? ProcMeshSections[SectionIndex].bSectionVisible : false;
}

int32 UProceduralMeshComponent::GetNumSections() const
{
	return ProcMeshSections.Num();
}

void UProceduralMeshComponent::AddCollisionConvexMesh(TArray<FVector> ConvexVerts)
{
	if(ConvexVerts.Num() >= 4)
	{ 
		// New element
		FKConvexElem NewConvexElem;
		// Copy in vertex info
		NewConvexElem.VertexData = ConvexVerts;
		// Update bounding box
		NewConvexElem.ElemBox = FBox(ConvexVerts);
		// Add to array of convex elements
		CollisionConvexElems.Add(NewConvexElem);
		// Refresh collision
		UpdateCollision();
	}
}

void UProceduralMeshComponent::ClearCollisionConvexMeshes()
{
	// Empty simple collision info
	CollisionConvexElems.Empty();
	// Refresh collision
	UpdateCollision();
}

void UProceduralMeshComponent::SetCollisionConvexMeshes(const TArray< TArray<FVector> >& ConvexMeshes)
{
	CollisionConvexElems.Reset();

	// Create element for each convex mesh
	for (int32 ConvexIndex = 0; ConvexIndex < ConvexMeshes.Num(); ConvexIndex++)
	{
		FKConvexElem NewConvexElem;
		NewConvexElem.VertexData = ConvexMeshes[ConvexIndex];
		NewConvexElem.ElemBox = FBox(NewConvexElem.VertexData);

		CollisionConvexElems.Add(NewConvexElem);
	}

	UpdateCollision();
}




*/


/*
void UProceduralMeshComponent::SetProcMeshSection(int32 SectionIndex, const FProcMeshSection& Section)
{
	// Ensure sections array is long enough
	if (SectionIndex >= ProcMeshSections.Num())
	{
		ProcMeshSections.SetNum(SectionIndex + 1, false);
	}

	ProcMeshSections[SectionIndex] = Section;

	UpdateLocalBounds(); // Update overall bounds
	UpdateCollision(); // Mark collision as dirty
	MarkRenderStateDirty(); // New section requires recreating scene proxy
}
*/





/*
void UProceduralMeshComponent::UpdateCollision()
{
	SCOPE_CYCLE_COUNTER(STAT_ProcMesh_UpdateCollision);

	UWorld* World = GetWorld();
	const bool bUseAsyncCook = World && World->IsGameWorld() && bUseAsyncCooking;

	if(bUseAsyncCook)
	{
		// Abort all previous ones still standing
		for (UBodySetup* OldBody : AsyncBodySetupQueue)
		{
			OldBody->AbortPhysicsMeshAsyncCreation();
		}

		AsyncBodySetupQueue.Add(CreateBodySetupHelper());
	}
	else
	{
        AsyncBodySetupQueue.Empty(); // If for some reason we modified the async at runtime, just clear any pending async body setups
        CreateProcMeshBodySetup();
	}
	
	UBodySetup* UseBodySetup = bUseAsyncCook ? AsyncBodySetupQueue.Last() : ProcMeshBodySetup;

	// Fill in simple collision convex elements
	UseBodySetup->AggGeom.ConvexElems = CollisionConvexElems;

	// Set trace flag
	UseBodySetup->CollisionTraceFlag = bUseComplexAsSimpleCollision ? CTF_UseComplexAsSimple : CTF_UseDefault;

	if(bUseAsyncCook)
	{
		UseBodySetup->CreatePhysicsMeshesAsync(FOnAsyncPhysicsCookFinished::CreateUObject(this, &UProceduralMeshComponent::FinishPhysicsAsyncCook, UseBodySetup));
	}
	else
	{
		// New GUID as collision has changed
		UseBodySetup->BodySetupGuid = FGuid::NewGuid();
		// Also we want cooked data for this
		UseBodySetup->bHasCookedCollisionData = true;
		UseBodySetup->InvalidatePhysicsData();
		UseBodySetup->CreatePhysicsMeshes();
		RecreatePhysicsState();
	}
}

void UProceduralMeshComponent::FinishPhysicsAsyncCook(bool bSuccess, UBodySetup* FinishedBodySetup)
{
	TArray<UBodySetup*> NewQueue;
	NewQueue.Reserve(AsyncBodySetupQueue.Num());

	int32 FoundIdx;
	if(AsyncBodySetupQueue.Find(FinishedBodySetup, FoundIdx))
	{
		if (bSuccess)
		{
			//The new body was found in the array meaning it's newer so use it
			ProcMeshBodySetup = FinishedBodySetup;
			RecreatePhysicsState();

			//remove any async body setups that were requested before this one
			for (int32 AsyncIdx = FoundIdx + 1; AsyncIdx < AsyncBodySetupQueue.Num(); ++AsyncIdx)
			{
				NewQueue.Add(AsyncBodySetupQueue[AsyncIdx]);
			}

			AsyncBodySetupQueue = NewQueue;
		}
		else
		{
			AsyncBodySetupQueue.RemoveAt(FoundIdx);
		}
	}
}







*/

/*

void UProceduralMeshComponent::CreateMeshSection(
    int32 SectionIndex, 
    const TArray<FVector>& Vertices, 
    const TArray<int32>& Triangles, 
    const TArray<FVector>& Normals, 
    const TArray<FVector2D>& UV0, 
    const TArray<FVector2D>& UV1, 
    const TArray<FVector2D>& UV2, 
    const TArray<FVector2D>& UV3, 
    const TArray<FColor>& VertexColors, 
    const TArray<FProcMeshTangent>& Tangents, 
    bool bCreateCollision
)
{
	SCOPE_CYCLE_COUNTER(STAT_ProcMesh_CreateMeshSection);

	// Ensure sections array is long enough
	if (SectionIndex >= ProcMeshSections.Num())
	{
		ProcMeshSections.SetNum(SectionIndex + 1, false);
	}

	// Reset this section (in case it already existed)
	FProcMeshSection& NewSection = ProcMeshSections[SectionIndex];
	NewSection.Reset();

	// Copy data to vertex buffer
	const int32 NumVerts = Vertices.Num();
	NewSection.ProcVertexBuffer.Reset();
	NewSection.ProcVertexBuffer.AddUninitialized(NumVerts);
	for (int32 VertIdx = 0; VertIdx < NumVerts; VertIdx++)
	{
		FProcMeshVertex& Vertex = NewSection.ProcVertexBuffer[VertIdx];

		Vertex.Position = Vertices[VertIdx];
		Vertex.Normal = (Normals.Num() == NumVerts) ? Normals[VertIdx] : FVector(0.f, 0.f, 1.f);
		Vertex.UV0 = (UV0.Num() == NumVerts) ? UV0[VertIdx] : FVector2D(0.f, 0.f);
		Vertex.UV1 = (UV1.Num() == NumVerts) ? UV1[VertIdx] : FVector2D(0.f, 0.f);
		Vertex.UV2 = (UV2.Num() == NumVerts) ? UV2[VertIdx] : FVector2D(0.f, 0.f);
		Vertex.UV3 = (UV3.Num() == NumVerts) ? UV3[VertIdx] : FVector2D(0.f, 0.f);
		Vertex.Color = (VertexColors.Num() == NumVerts) ? VertexColors[VertIdx] : FColor(255, 255, 255);
		Vertex.Tangent = (Tangents.Num() == NumVerts) ? Tangents[VertIdx] : FProcMeshTangent();

		// Update bounding box
		NewSection.SectionLocalBox += Vertex.Position;
	}

	// Get triangle indices, clamping to vertex range
	const int32 MaxIndex = NumVerts - 1;
	const auto GetTriIndices = [&Triangles, MaxIndex](int32 Idx)
	{
		return TTuple<int32, int32, int32>(FMath::Min(Triangles[Idx    ], MaxIndex),
										   FMath::Min(Triangles[Idx + 1], MaxIndex),
			                               FMath::Min(Triangles[Idx + 2], MaxIndex));
	};

	const int32 NumTriIndices = (Triangles.Num() / 3) * 3; // Ensure number of triangle indices is multiple of three

	// Detect degenerate triangles, i.e. non-unique vertex indices within the same triangle
	int32 NumDegenerateTriangles = 0;
	for (int32 IndexIdx = 0; IndexIdx < NumTriIndices; IndexIdx += 3)
	{
		int32 a, b, c;
		Tie(a, b, c) = GetTriIndices(IndexIdx);
		NumDegenerateTriangles += a == b || a == c || b == c;
	}
	if (NumDegenerateTriangles > 0)
	{
		UE_LOG(LogProceduralComponent, Warning, TEXT("Detected %d degenerate triangle%s with non-unique vertex indices for created mesh section in '%s'; degenerate triangles will be dropped."),
			   NumDegenerateTriangles, NumDegenerateTriangles > 1 ? TEXT("s") : TEXT(""), *GetFullName());
	}

	// Copy index buffer for non-degenerate triangles
	NewSection.ProcIndexBuffer.Reset();
	NewSection.ProcIndexBuffer.AddUninitialized(NumTriIndices - NumDegenerateTriangles * 3);
	int32 CopyIndexIdx = 0;
	for (int32 IndexIdx = 0; IndexIdx < NumTriIndices; IndexIdx += 3)
	{
		int32 a, b, c;
		Tie(a, b, c) = GetTriIndices(IndexIdx);

		if (a != b && a != c && b != c)
		{
			NewSection.ProcIndexBuffer[CopyIndexIdx++] = a;
			NewSection.ProcIndexBuffer[CopyIndexIdx++] = b;
			NewSection.ProcIndexBuffer[CopyIndexIdx++] = c;
		}
		else
		{
			--NumDegenerateTriangles;
		}
	}
	check(NumDegenerateTriangles == 0);
	check(CopyIndexIdx == NewSection.ProcIndexBuffer.Num());

	NewSection.bEnableCollision = bCreateCollision;

	UpdateLocalBounds(); // Update overall bounds
	UpdateCollision(); // Mark collision as dirty
	MarkRenderStateDirty(); // New section requires recreating scene proxy
}
*/
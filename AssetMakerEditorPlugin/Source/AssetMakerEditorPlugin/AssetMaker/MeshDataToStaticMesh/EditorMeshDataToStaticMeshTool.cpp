#include "EditorMeshDataToStaticMeshTool.h"

#include "Engine/StaticMesh.h"
#include "Factories/Factory.h"
#include "StaticMeshAttributes.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Subsystems/AssetEditorSubsystem.h"

#include "MeshDescription.h"
#include "StaticMeshAttributes.h"
#include "AssetRegistry/AssetRegistryModule.h"

#include "Misc/PackageName.h"
#include "UObject/Package.h"
#include "UObject/SavePackage.h"
#include "PackageUtils/PackageUtils.h"  // für FSavePackageArgs

TArray<MeshDataAssetTask> UEditorMeshDataToStaticMeshTool::tasks;

void UEditorMeshDataToStaticMeshTool::AppendTask(const MeshDataAssetTask &task){
    tasks.Add(task);
}

void UEditorMeshDataToStaticMeshTool::Log(FString msg){
    UE_LOG(LogTemp, Warning, TEXT("UEditorMeshDataToStaticMeshTool %s"), *msg);
}


bool UEditorMeshDataToStaticMeshTool::AssetAlreadyExists(FString path, FString nameAsset){
    // Pfad korrekt zusammensetzen: "StaticMesh'/Game/terrainPlugin/Prefabs/foliage/grassAssetDefault.grassAssetDefault'"
    FString combined = FString::Printf(TEXT("StaticMesh'%s%s.%s'"), *path, *nameAsset, *nameAsset);
    Log(combined);

    // UStaticMesh laden
    UStaticMesh* mesh = LoadObject<UStaticMesh>(nullptr, *combined);
    return mesh != nullptr;
}

void UEditorMeshDataToStaticMeshTool::Launch(){
    UEditorMeshDataToStaticMeshTool *tool = NewObject<UEditorMeshDataToStaticMeshTool>(); //is not executed
    if(tool){
        Log(FString::Printf(TEXT("Launch, tasks %d"), tasks.Num()));
        TArray<MeshDataAssetTask> unfinished;
        for (int i = 0; i < tasks.Num(); i++)
        {
            MeshDataAssetTask &current = tasks[i];
            if(current.IsValidTask()){
                if(AssetAlreadyExists(current.path(), current.name())){
                    Log(FString::Printf(TEXT("Task Skipped, already created %s"), *current.pathComplete()));
                }else{
                    Log(FString::Printf(TEXT("Task executing %s"), *current.pathComplete()));
                    UStaticMesh *ptr = tool->CreateStaticMeshAsset(
                        current.pathComplete(),
                        current.GetVertexBuffer(),
                        current.GetTriangleBuffer()
                    );
                    if(!ptr){
                        unfinished.Add(current);
                    }
                
                }
            }
        }

        tasks.Empty();
        tasks = unfinished;
        /*FString path = TEXT("Game/Generated/newmesh");
        TArray<FVector> vertecies;
        TArray<int32> triangles;
        UStaticMesh *ptr = tool->CreateStaticMeshAsset(path, vertecies, triangles);*/
    }
}

UStaticMesh* UEditorMeshDataToStaticMeshTool::CreateStaticMeshAsset(
    const FString& AssetPath, 
    const TArray<FVector>& vertices, 
    const TArray<int32>& triangles
){
    Log("CreateStaticMeshAsset:: Launch Creation");
    // Create a package for the asset
    UPackage* Package = CreatePackage(*AssetPath);
    if(!Package){
        Log("CreateStaticMeshAsset:: failed to load pacakge");
    }

    Package->FullyLoad();

    // Create StaticMesh object
    UStaticMesh* StaticMesh = NewObject<UStaticMesh>(
        Package, 
        *FPaths::GetBaseFilename(AssetPath), 
        RF_Public | RF_Standalone
    );
    if(!Package){
        Log("CreateStaticMeshAsset:: failed to create ustaticmesh");
    }


    int32 lod = 0;
    FMeshDescription tmp;
    FMeshDescription *description = GetLod(lod, StaticMesh);

    /// Todo: ---- make mesh data -----
    if(description == nullptr){
        Log("CreateStaticMeshAsset:: failed to load lod, is nullptr");
        return nullptr;
    }

    Log("CreateStaticMeshAsset:: Mesh description valid!");
    Append(description, vertices, triangles);




    TArray<const FMeshDescription *> lods = {description};
    if(Build(lods, StaticMesh)){
        //save in engine yourself.

        //debug dont return anything
    }

    return nullptr;
}


FMeshDescription *UEditorMeshDataToStaticMeshTool::GetLod(int32 lod, UStaticMesh *mesh){
    if (!mesh){
        Log("Did not create mesh description, UStaticMesh nullptr!");
        return nullptr;
    }

    //this is correct, doc doesnt tell how to do this.
    //dont ask me how i did it.
    FStaticMeshSourceModel &SourceModel = mesh->AddSourceModel(); // fügt LOD0 hinzu
    return SourceModel.CreateMeshDescription();
}

bool UEditorMeshDataToStaticMeshTool::Build(TArray<const FMeshDescription*> lods, UStaticMesh *mesh){
    /*
    UStaticMesh::
    bool	
    BuildFromMeshDescriptions ( const TArray< const FMeshDescription* >& MeshDescriptions,
    const FBuildMeshDescriptionsParams& Params //??
    )

    Builds static mesh render buffers from a list of MeshDescriptions, one per LOD.
    */
    return mesh->BuildFromMeshDescriptions(lods);
}



void UEditorMeshDataToStaticMeshTool::Append(
    FMeshDescription *mesh,
    const TArray<FVector>& vertices, 
    const TArray<int32>& triangles
){
    if(mesh){
        TArray<FVertexInstanceID> vertexBufferAsIdBuffer;
        AppendAllVertecies(*mesh, vertices, vertexBufferAsIdBuffer);

        //return; //debug
        FPolygonGroupID materialId = MakeMaterialLayer(*mesh);
        AppendAllTriangles(
            *mesh,
            vertexBufferAsIdBuffer,
            triangles,
            materialId
        );
    }
}



void UEditorMeshDataToStaticMeshTool::AppendAllVertecies(
    FMeshDescription &mesh,
    const TArray<FVector> &vertexBuffer,
    TArray<FVertexInstanceID> &outBuffer
){
    outBuffer.Empty();
    FStaticMeshAttributes Attributes(mesh);
    Attributes.Register();

    TArray<FVertexID> vertexIdBuffer;
    for (int i = 0; i < vertexBuffer.Num(); i++){
        vertexIdBuffer.Add(mesh.CreateVertex());
    }

    TVertexAttributesRef<FVector3f> vertexPositions = mesh.GetVertexPositions();
    for (int i = 0; i < vertexIdBuffer.Num(); i++){
        vertexPositions[vertexIdBuffer[i]] = FVector3f(
            float(vertexBuffer[i].X),
            float(vertexBuffer[i].Y),
            float(vertexBuffer[i].Z)
        );
    }


    //create new instance for each vertex
    for (int i = 0; i < vertexIdBuffer.Num(); i++){
        outBuffer.Add(
            mesh.CreateVertexInstance(vertexIdBuffer[i])
        );
    }
}

FPolygonGroupID UEditorMeshDataToStaticMeshTool::MakeMaterialLayer(
    FMeshDescription &mesh
){
    return mesh.CreatePolygonGroup();
}


void UEditorMeshDataToStaticMeshTool::AppendAllTriangles(
    FMeshDescription &mesh,
    TArray<FVertexInstanceID> &vertexIdBuffer,
    const TArray<int32>& triangles,
    FPolygonGroupID &materialId
){
    /*
    FMeshDescription::
    FTriangleID	
    CreateTriangle ( 
        const FPolygonGroupID PolygonGroupID,
        TArrayView< const FVertexInstanceID > VertexInstanceIDs,
        TArray< FEdgeID >* OutEdgeIDs
    )*/
    for (int t = 2; t < triangles.Num(); t += 3){
        int32 t0 = triangles[t - 2];
        int32 t1 = triangles[t - 1];
        int32 t2 = triangles[t];
        if(TriangleIsValid(t0, t1, t2, vertexIdBuffer)){
            TArray<FVertexInstanceID> triangleCurrent = {
                vertexIdBuffer[t0],
                vertexIdBuffer[t1],
                vertexIdBuffer[t2]
            };

            TArray<FEdgeID> OutEdgeIDs;//ignored.
            FTriangleID ignored = mesh.CreateTriangle(
                materialId,
                triangleCurrent,
                &OutEdgeIDs
            );
        }
    }
}

bool UEditorMeshDataToStaticMeshTool::TriangleIsValid(
    int32 t0, int32 t1, int32 t2,
    TArray<FVertexInstanceID> &vertexIdBuffer
){
    return 
    t0 >= 0 && t0 < vertexIdBuffer.Num() && 
    t1 >= 0 && t1 < vertexIdBuffer.Num() &&
    t2 >= 0 && t2 < vertexIdBuffer.Num();
}

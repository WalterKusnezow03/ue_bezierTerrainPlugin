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
            UStaticMesh *ptr = tool->CreateStaticMeshAsset(current);
            if(!ptr){
                unfinished.Add(current);
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

bool UEditorMeshDataToStaticMeshTool::TaskCanBeExecuted(MeshDataAssetTask &task){
    if(task.IsValidTask()){
        if(AssetAlreadyExists(task.path(), task.name())){
            Log(FString::Printf(TEXT("Task Skipped, already created %s"), *task.pathComplete()));
            return false;
        }
    }else{
        Log("CreateStaticMeshAsset:: Task Invalid!");
        return false;
    }
    //never happens.
    return true;
}

UStaticMesh* UEditorMeshDataToStaticMeshTool::CreateStaticMeshAsset(MeshDataAssetTask &task){
    Log("CreateStaticMeshAsset:: Launch Creation");


    //check if task ok
    if(!TaskCanBeExecuted(task)){
        return nullptr;
    }

    // Create a package for the asset
    UPackage* Package = CreatePackage(*task.pathComplete());
    if(!Package){
        Log("CreateStaticMeshAsset:: failed to load pacakge");
    }

    Package->FullyLoad();

    // Create StaticMesh object
    UStaticMesh* StaticMesh = NewObject<UStaticMesh>(
        Package, 
        *FPaths::GetBaseFilename(*task.pathComplete()), 
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
    Append(description, task);




    TArray<const FMeshDescription *> lods = {description};
    if(Build(lods, StaticMesh)){
        //save in engine yourself.
        //debug dont return anything (allows recreation, task not cleared)
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
    MeshDataAssetTask &task
){
    if(mesh){
        TArray<FVertexInstanceID> vertexBufferAsIdBuffer;
        AppendAllVertecies(*mesh, task.GetVertexBuffer(), vertexBufferAsIdBuffer);

        //return; //debug
        FPolygonGroupID materialId = MakeMaterialLayer(*mesh);
        AppendAllTriangles(
            *mesh,
            vertexBufferAsIdBuffer,
            task.GetTriangleBuffer(),
            materialId
        );


        int32 uvChannel = 0;
        AppendUVBuffer(
            *mesh,
            task,
            vertexBufferAsIdBuffer,
            uvChannel 
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




void UEditorMeshDataToStaticMeshTool::AppendUVBuffer(
    FMeshDescription &mesh,
    MeshDataAssetTask &task,
    TArray<FVertexInstanceID> &vertexIdBuffer,
    int32 uvChannel 
){

    TArray<FVector2D> &uvBuffer = task.GetUV0();
    if (uvBuffer.Num() <= 0)
    {
        return;
    }

    mesh.CreateUV(uvChannel);

    FStaticMeshAttributes Attributes(mesh); // can be created as often as you want, just a funny helper for the same FMeshDescription.
    Attributes.Register();
    TVertexInstanceAttributesRef<FVector2f> InstanceUVs = Attributes.GetVertexInstanceUVs();
    for (int i = 0; i < vertexIdBuffer.Num(); i++)
    {
        if (i < uvBuffer.Num())
        {
            FVector2D uvCurrent = uvBuffer[i];
            task.ModifyUvBasedOnFlags(uvCurrent);

            InstanceUVs.Set(vertexIdBuffer[i], uvChannel, FVector2f(uvCurrent.X, uvCurrent.Y));
        }
    }

}
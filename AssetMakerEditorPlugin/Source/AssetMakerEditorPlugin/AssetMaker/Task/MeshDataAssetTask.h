#pragma once 

#include "CoreMinimal.h"

///class to track asset maker tasks (Path + mesh data.)
class ASSETMAKEREDITORPLUGIN_API MeshDataAssetTask {

public:
    MeshDataAssetTask();
    ~MeshDataAssetTask();

    MeshDataAssetTask(const MeshDataAssetTask &other);
    MeshDataAssetTask &operator=(const MeshDataAssetTask &other);

    void Update(const TArray<FVector> &vertexBuffer, const TArray<int32> &triangleBuffer);


    /// @brief path must start and end with "/"
    /// @param path /somePlugin/path/inner/
    /// @param name somename
    void UpdatePathAndName(const FString path, const FString name);


    FString path(){
        return filePath;
    }
    FString name(){
        return fileName;
    }

    FString pathComplete();
    TArray<FVector> &GetVertexBuffer();
    TArray<int32> &GetTriangleBuffer();

    bool IsValidTask();

private:
    FString filePath;
    FString fileName;
    TArray<FVector> vertexBufferSaved;
    TArray<int32> triangleBufferSaved;
};
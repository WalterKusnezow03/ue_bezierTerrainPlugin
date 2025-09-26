#pragma once 

#include "CoreMinimal.h"

///class to track asset maker tasks (Path + mesh data.)
///will not make any path or buffer save. Expected to be correct.
class ASSETMAKEREDITORPLUGIN_API MeshDataAssetTask {

public:
    MeshDataAssetTask();
    ~MeshDataAssetTask();

    MeshDataAssetTask(const MeshDataAssetTask &other);
    MeshDataAssetTask &operator=(const MeshDataAssetTask &other);

    void Update(const TArray<FVector> &vertexBuffer, const TArray<int32> &triangleBuffer);
    void Update(
        const TArray<FVector> &vertexBuffer,
        const TArray<int32> &triangleBuffer,
        const TArray<FVector2D> &uvBufferIn
    );

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
    TArray<FVector2D> &GetUV0();

    bool IsValidTask();


    /// @brief manual marking needed, in unreal its the other way around (is by default true,
    ///means (0,0) is bl, (1,1) is tr)
    void MarkXUv_0Left(bool flag);
    void MarkYUv_0Bottom(bool flag);

    void ModifyUvBasedOnFlags(FVector2D &uv);

private:
    FString filePath;
    FString fileName;
    TArray<FVector> vertexBufferSaved;
    TArray<int32> triangleBufferSaved;
    TArray<FVector2D> uvBufferSaved;

    bool uvX_0left = false;
    bool uvY_0Up = true;

};
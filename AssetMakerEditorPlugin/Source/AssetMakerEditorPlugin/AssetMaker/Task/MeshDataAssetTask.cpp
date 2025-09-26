#include "MeshDataAssetTask.h"


MeshDataAssetTask::MeshDataAssetTask(){

}

MeshDataAssetTask::~MeshDataAssetTask(){

}

MeshDataAssetTask::MeshDataAssetTask(const MeshDataAssetTask &other){
    if(this != &other){
        *this = other;
    }
}

MeshDataAssetTask &MeshDataAssetTask::operator=(const MeshDataAssetTask &other){
    if(this != &other){
        Update(other.vertexBufferSaved, other.triangleBufferSaved, other.uvBufferSaved);
        UpdatePathAndName(other.filePath, other.fileName);
        uvX_0left = other.uvX_0left;
        uvY_0Up = other.uvY_0Up;
    }
    return *this;
}

bool MeshDataAssetTask::IsValidTask(){
    if(filePath.Len() <= 0){
        return false;
    }
    if(fileName.Len() <= 0){
        return false;
    }
    if(vertexBufferSaved.Num() < 3){
        return false;
    }
    if(triangleBufferSaved.Num() < 3){
        return false;
    }
    return true;
}

void MeshDataAssetTask::Update(const TArray<FVector> &vertexBuffer, const TArray<int32> &triangleBuffer){
    vertexBufferSaved = vertexBuffer;
    triangleBufferSaved = triangleBuffer;
    uvBufferSaved.Empty();
}

void MeshDataAssetTask::Update(
    const TArray<FVector> &vertexBuffer, 
    const TArray<int32> &triangleBuffer,
    const TArray<FVector2D> &uvBufferIn 
){
    vertexBufferSaved = vertexBuffer;
    triangleBufferSaved = triangleBuffer;
    uvBufferSaved = uvBufferIn;
}

void MeshDataAssetTask::UpdatePathAndName(const FString path, const FString name){
    filePath = path;
    fileName = name;
}

//unclear if this is correct
FString MeshDataAssetTask::pathComplete(){
    FString output = filePath; //must have "/" at end
    output += fileName;
    //output += ".";
    //output += fileName;
    return output;
}

TArray<FVector> &MeshDataAssetTask::GetVertexBuffer(){
    return vertexBufferSaved;
}

TArray<int32> &MeshDataAssetTask::GetTriangleBuffer(){
    return triangleBufferSaved;
}

TArray<FVector2D> &MeshDataAssetTask::GetUV0(){
    return uvBufferSaved;
}

void MeshDataAssetTask::MarkXUv_0Left(bool flag){
    uvX_0left = flag;
}
void MeshDataAssetTask::MarkYUv_0Bottom(bool flag){
    uvY_0Up = flag;
}

void MeshDataAssetTask::ModifyUvBasedOnFlags(FVector2D &uv){
    if(uvX_0left){
        uv.X = 1.0f - uv.X;
    }
    if(uvY_0Up){
        uv.Y = 1.0 - uv.Y;
    }
}
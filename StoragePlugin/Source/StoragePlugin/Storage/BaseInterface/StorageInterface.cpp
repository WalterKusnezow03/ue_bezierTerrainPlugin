#include "StorageInterface.h"
#include "Misc/FileHelper.h" //save load
#include "HAL/FileManager.h" //remove
#include "StoragePlugin/Storage/VertexData/TerrainVertex.h"
#include "StoragePlugin/Storage/VertexData/TerrainNormal.h"
#include "StoragePlugin/Storage/VertexData/TerrainUv.h"
#include "StoragePlugin/Storage/VertexData/TerrainVertexIndex.h"
#include "GameCore/DebugHelper.h"


//-------- SAVING --------

bool StorageInterface::SaveBinaryData(
    FString Path,
    TArray<uint8> &Bytes
){
    // Load
    return FFileHelper::SaveArrayToFile(Bytes, *Path);
}


//-------- LOADING --------



//load binary
bool StorageInterface::LoadBinaryData(
    FString Path,
    TArray<uint8> &Bytes
){
    // Load
    return FFileHelper::LoadFileToArray(Bytes, *Path);
}



// ------ PATH -------

/// @brief creates a path as "/ProjectDir/worldLevelName/", "/" at end, append inner path.
/// @param worldLevelName 
/// @return 
FString StorageInterface::BaseDir(FString worldLevelName){
    FString preString = BaseDir(); //ends with a single "/"
    preString += FString::Printf(TEXT("%s/"), *worldLevelName); //also ends with a single "/"
    return preString;
}


FString StorageInterface::BaseDir(){
    return FPaths::ProjectSavedDir() //has "/" at end
    + TEXT("StorageInterfaceSaved/");
}


// ----- REMOVE DIR ------

/// @brief requieres a subdir path to start with the name, no trailing slash needed
/// @param subDir 
void StorageInterface::RemoveSubDir(FString subDir){
    FString completePath = BaseDir() + subDir;
    if(!SubDirAllowed(subDir)){
        DebugHelper::logMessage("StorageInterface Remove Failed: Subdir illegal ", subDir);
    }

    /*
    Known Issue: Doesnt delete the directory itself for reasons i dont understand!
    Also the print doesnt appear but content of folders ARE removed.    
    */
    IFileManager& fileManager = IFileManager::Get();
    if(fileManager.DirectoryExists(*completePath)){
        bool deleteRecursive = true; //yes, if not enabled, and the folder is not empty, nothign will happen.
        // true = rekursiv löschen
        if(fileManager.DeleteDirectory(
            *completePath, 
            /*RequireExists= (no exception thrown if false)*/false, 
            /*Tree=*/deleteRecursive
        )){
            DebugHelper::logMessage("StorageInterface Removed inner content of Directory: ", completePath);
        }

        if(fileManager.DeleteDirectory(
            *completePath, 
            /*RequireExists= (no exception thrown if false)*/false, 
            /*Tree=*/false
        )){
            DebugHelper::logMessage("StorageInterface Removed root of Directory: ", completePath);
        }


    }

}

bool StorageInterface::SubDirAllowed(FString subDir)
{
    FString Normalized = BaseDir() + subDir;
    FPaths::NormalizeDirectoryName(Normalized);
    FPaths::CollapseRelativeDirectories(Normalized);

    FString Root = BaseDir();
    FPaths::NormalizeDirectoryName(Root);

    // Prüfen, dass Normalized mit Root beginnt
    return Normalized.StartsWith(Root);
}

// ------ HELPERS -------

void StorageInterface::PrintBinary(TArray<uint8>&bytes, FString message){
    FString byteString = FString::Printf(TEXT("Storage Interface %s bin: "), *message);
    for (int i = 0; i < bytes.Num(); i++){
        byteString += FString::FromInt((int32)bytes[i]);
    }

    DebugHelper::logMessage(byteString);
}
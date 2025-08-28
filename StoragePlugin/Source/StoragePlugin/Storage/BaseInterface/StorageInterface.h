#pragma once

#include "CoreMinimal.h"

class STORAGEPLUGIN_API StorageInterface {

public:
    StorageInterface() {};
    virtual ~StorageInterface() {};

    /// @brief removes a sub dir in saved folder. Start with the name, no prefix or trianling "/" needed.
    /// @param subDir 
    void RemoveSubDir(FString subDir);

protected:
    bool SubDirAllowed(FString subDir);

    bool SaveBinaryData(
        FString path,
        TArray<uint8> &Bytes
    );

    bool LoadBinaryData(
        FString Path,
        TArray<uint8> &Bytes
    );

    /// @brief will create a path with the world string, ends with a slash "/": somepath/-appendyourinnerPath-
    FString BaseDir(FString worldLevelName);

    /// @brief will create a path within the project save dir "/" at end.
    FString BaseDir();


    void PrintBinary(TArray<uint8> &bytes, FString message);
};
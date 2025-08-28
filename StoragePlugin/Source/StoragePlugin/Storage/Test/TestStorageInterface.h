#pragma once

#include "CoreMinimal.h"
#include "StoragePlugin/Storage/StringData/StorageInterfaceStringArray.h"
#include "StoragePlugin/Storage/VertexData/StorageInterface/StorageInterfaceMeshData.h"

class STORAGEPLUGIN_API TestStorageInterface {
    
public:
    TestStorageInterface(){};
    ~TestStorageInterface(){};


    void Test(){
        StorageInterfaceStringArray stringInterface;
        stringInterface.Test();

        StorageInterfaceMeshData meshDataInterface;
        meshDataInterface.Test();
    }
};
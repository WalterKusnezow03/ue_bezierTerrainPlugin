// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetPlugin/gamestart/assetManager.h"
#include "AssetPlugin/gamestart/TAssetManager/assetManagerGeneric.h"

#include "CoreMinimal.h"


assetManager *assetManager::instancePointer = nullptr;


void assetManager::EndGame(){
    if(assetManager* ptr = instance()){
        delete ptr;
        assetManager::instancePointer = nullptr;
    }
}

/// @brief you are not allowed to delete this pointer!
/// @return instance pointer
assetManager* assetManager::instance(){
    if(assetManager::instancePointer == nullptr){
        assetManager::instancePointer = new assetManager();
    }
    return assetManager::instancePointer;
}



void assetManager::setupDefaultTracker(){
    /*
    Track<entityEnum>();
    Track<weaponEnum>();
    Track<throwableEnum>();
    Track<particleEnum>();
    Track<materialEnum>();
    Track<textureEnum>();
    */
}

void assetManager::Clear(){
    for(auto &pair : TrackedEnums){
        FNamedEnumBase *ptr = pair.second;
        if(ptr){
            delete ptr;
            pair.second = nullptr;
        }
    }
    TrackedEnums.clear();


    for(auto &pair : TrackedMap){
        assetManagerGeneric<FString, UObject> *ptr = pair.second;
        if(ptr){
            delete ptr;
            pair.second = nullptr;
        }
    }
    TrackedMap.clear();

    for(auto &pair : TrackedDoubleKeyMap){
        assetManagerTwoGeneric<FString, FString, UObject> *ptr = pair.second;
        if(ptr){
            delete ptr;
            pair.second = nullptr;
        }
    }
    TrackedDoubleKeyMap.clear();
}

assetManager::assetManager()
{
    setupDefaultTracker();
}

assetManager::~assetManager()
{
    Clear();
}


void assetManager::setDebugCubeBp(UClass *uclassin){
    if(uclassin != nullptr){
        debugCubePointer = uclassin;
    }
}
UClass *assetManager::debugCubeBp(){
    return debugCubePointer;
}
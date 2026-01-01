// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetLoader.h"



AssetLoader::AssetLoader()
{
}

AssetLoader::~AssetLoader()
{
}




/// @brief Method to load a Blueprint class from a path.
/// @param path The path to the Blueprint class.
/// @return The loaded UClass, or nullptr if it fails.
/// works as expected
UClass* AssetLoader::loadUClassBluePrint(FString path){
    // Load the class object dynamically
    UClass* bpClass = StaticLoadClass(UObject::StaticClass(), nullptr, *path);
    
    
    // Check if the class was loaded successfully
    if (bpClass != nullptr)
    {
        return bpClass; //issues can still occur here when a non blueprint is found but treated like one!
    }
    return nullptr;
}



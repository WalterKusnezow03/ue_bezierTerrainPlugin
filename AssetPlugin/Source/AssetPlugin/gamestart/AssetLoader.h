// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * will load all nesecarry assets, bots, weapons etc
 * must be called from gamemode class constructor
 * MUST BE THE FIRST THING LOADED WHEN LAUNCHING THE GAME
 */
class ASSETPLUGIN_API AssetLoader
{
public:
	AssetLoader(UWorld *worldIn);
	~AssetLoader();

	void loadAssets();

	///@brief Example path: TEXT("Blueprint'/Game/Prefabs/terrain/materials/texturedMaterial.texturedMaterial'")
	/// or
	/// TEXT("Texture2D'/Game/Prefabs/textures/partone'");
	/// TEXT("<DT>"'path/to/asset'");
	template <typename T>
	static T* loadAsset(FString path){
		T* bpObject = LoadObject<T>(nullptr, *path);
		
		// Check if the class was loaded successfully
		if (bpObject != nullptr)
		{
			return bpObject;
		}
		return nullptr;
	}
	
	
	


private:
	class UWorld *world;

	//loads a uclass from a specific path
	UClass* loadUClassBluePrint(FString path);
	UMaterial *loadMaterial(FString path);

	FString buildPath(FString path, FString bluePrintName);
	void cleanUpPath(FString &s);

	void loadWeaponAttachments();
	void loadWeapons();
	void loadEntities();
	void loadThrower();
	void loadParticles();
	void loadMaterials();
	void loadUI();
	void loadTextures();

	void loadDebugCube();
};

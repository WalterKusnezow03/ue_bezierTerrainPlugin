// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetPlugin/gamestart/PathMaker/enum/EAssetType.h"

#include "AssetPlugin/gamestart/PathMaker/AssetPathMaker.h"
#include "AssetPlugin/gamestart/PathMaker/enum/EAssetType.h"
#include "assetManager.h"

/**
 * will load all nesecarry assets, bots, weapons etc
 * must be called from gamemode class begin play
 * 
 * --> create an stack instance, assets are saved and hidden from GC inside 
 * the asset manager.
 * Use: LoadAndSaveAssetToManager<Enum, T_isA_UObject> to load assets later if needed.
 * Assets can be found with the Enum from asset manager.
 * 
 * MUST BE ONE OF THE FIRST THINGS LOADED WHEN LAUNCHING THE GAME
 */
class ASSETPLUGIN_API AssetLoader
{
public:
	AssetLoader();
	~AssetLoader();
	

	///@brief Example path: TEXT("Blueprint'/Game/Prefabs/terrain/materials/texturedMaterial.texturedMaterial'")
	/// or
	/// TEXT("Texture2D'/Game/Prefabs/textures/partone.partone'");
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
	


	/// ---- LOAD AND ADD ASSET TO ASSET MANAGER ----

	/// @brief saves an loaded asset with the given enum and key to the asset manager instance
	/// @tparam E Enum to track in Asset Manager (Entity Enum , Weapon Enum, Texture Enum etc)
	/// @tparam T Some Type T, UTexture2D, UClass, UMaterial
	/// @param enumValue key for Enum to track with
	/// @param pluginName 
	/// @param innerPath 
	/// @param assetName 
	template <typename E, typename T>
	static void LoadAndSaveAssetToManager(
		E enumValue, //track in asset manager
		FString pluginName, // like "Game" for game or any other plugin name
		FString innerPath, // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
		FString assetName //Just the file name as displayed
	){
		EAssetType typeAsset;
		if(FindType<T>(typeAsset)){
			//make a path
			AssetPathMaker pathMaker;
			FString path = pathMaker.makeAssetPath(typeAsset, pluginName, innerPath, assetName);

			//find asset
			T *loaded = nullptr;

			//explicit class loading
			if(typeAsset == EAssetType::EUClassBlueprint){
				UClass *loadedUClass = loadUClassBluePrint(path);
				if(loadedUClass){
					loaded = Cast<T>(loadedUClass);
				}
			}else{
				//other asset loading (Any asset, but not for blueprint uclasses)
				loaded = loadAsset<T>(path);
			}

			//save asset to asset manager once found
			if(loaded != nullptr){
				assetManager *manager = assetManager::instance();
				if(manager){
					manager->Add<E, T>(enumValue, loaded);
				}
			}
		}
	}



	/// @brief saves an loaded asset with the given enum and keys (both!) to the asset manager instance
	/// @tparam E0 and E1 Enum to track in Asset Manager (Entity Enum , Weapon Enum, Texture Enum etc)
	/// @tparam T Some Type T, UTexture2D, UClass, UMaterial
	/// @param enumValue key for Enum to track with
	/// @param pluginName 
	/// @param innerPath 
	/// @param assetName 
	template <typename E0, typename E1, typename T>
	static void LoadAndSaveAssetToManager(
		E0 key0, //track in asset manager
		E1 key1, //track in asset manager
		FString pluginName, // like "Game" for game or any other plugin name
		FString innerPath, // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
		FString assetName //Just the file name as displayed
	){
		EAssetType typeAsset;
		if(FindType<T>(typeAsset)){
			//make a path
			AssetPathMaker pathMaker;
			FString path = pathMaker.makeAssetPath(typeAsset, pluginName, innerPath, assetName);

			//find asset
			T *loaded = nullptr;

			//explicit class loading
			if(typeAsset == EAssetType::EUClassBlueprint){
				UClass *loadedUClass = loadUClassBluePrint(path);
				if(loadedUClass){
					loaded = Cast<T>(loadedUClass);
				}
			}else{
				//other asset loading (Any asset, but not for blueprint uclasses)
				loaded = loadAsset<T>(path);
			}

			//save asset to asset manager once found
			if(loaded != nullptr){
				assetManager *manager = assetManager::instance();
				if(manager){
					manager->Add<E0, E1, T>(key0, key1, loaded);
				}
			}
		}
	}







private:

	/// @brief finds the Easset type for a Template Type T, must be UObject derived,
	/// only works for UClass, UMaterials, UTexture2D for now.
	/// @tparam T 
	/// @param outType 
	/// @return 
	template <typename T> 
	static bool FindType(EAssetType &outType){
		/*
		UObjectBase
		└── UObjectBaseUtility
			└── UObject
				├── UField
				│   └── UStruct
				│       └── UClass
				│
				├── UMaterialInterface
				│   └── UMaterial
				│
				└── UTexture
					└── UTexture2D
		*/



		bool found = false;
		if constexpr (std::is_base_of<UClass, T>::value) //isA check on compile time 
		{
			outType = EAssetType::EUClassBlueprint;
			found = true;
		}
		else if constexpr (std::is_base_of<UMaterial, T>::value) {
			outType = EAssetType::EUMaterial;
			found = true;
		}
		else if constexpr (std::is_base_of<UTexture2D, T>::value){
			outType = EAssetType::EUTexture2D;
			found = true;
		}
		else if constexpr (std::is_base_of<UStaticMesh, T>::value){
			outType = EAssetType::EUStaticMesh;
			found = true;
		}
		else {
			found = false;
		}
		return found;
	}

	

	//loads a uclass from a specific path
	static UClass* loadUClassBluePrint(FString path);
	


	void loadWeaponAttachments();
	void loadWeapons();
	void loadEntities();
	void loadThrowables();
	void loadParticles();
	void loadMaterials();
	
	void loadTextures();

	void loadDebugCube();
};

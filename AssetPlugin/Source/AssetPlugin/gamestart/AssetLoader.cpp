// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetLoader.h"

#include "AssetPlugin/gamestart/assetEnums/materialEnum.h"




AssetLoader::AssetLoader()
{
}

AssetLoader::~AssetLoader()
{
}


//constructor
void AssetLoader::loadAssets()
{

    loadMaterials(); //materials need to be loaded first to not have any issues
    loadEntities();

    loadWeapons();
    loadWeaponAttachments();
    loadThrowables();
    loadParticles();
    
    loadTextures();
    

    loadDebugCube();
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





/// @brief load all entities 
void AssetLoader::loadEntities(){

    LoadAndSaveAssetToManager<entityEnum, UClass>(
        entityEnum::entity_enum, // track in asset manager
        "Game",                  // like "Game" for game or any other plugin name
        "Prefabs/player",        // like: "Prefabs/Weapons/attachments", no trailing slash
        "entityPrefab"           // Just the file name as displayed
    );

    LoadAndSaveAssetToManager<entityEnum, UClass>(
        entityEnum::human_enum, // track in asset manager
        "Game",                  // like "Game" for game or any other plugin name
        "Prefabs/player",        // like: "Prefabs/Weapons/attachments", no trailing slash
        "humanEntityPrefab"           // Just the file name as displayed
    );



    /*
    if(assetManager *am = assetManager::instance()){
        FString path = FString::Printf(TEXT("/Game/Prefabs/player/"));
        FString entityString = buildPath(path, "entityPrefab");
        am->addBp(entityEnum::entity_enum, loadUClassBluePrint(entityString));


        FString humanString = FString::Printf(TEXT("Blueprint'/Game/Prefabs/player/humanEntityPrefab.humanEntityPrefab_C'"));
        am->addBp(entityEnum::human_enum, loadUClassBluePrint(humanString));
    
    }*/


}








void AssetLoader::loadWeaponAttachments(){ 
    /*
    static void LoadAndSaveAssetToManager(
		E0 key0, //track in asset manager
		E1 key1, //track in asset manager
		FString pluginName, // like "Game" for game or any other plugin name
		FString innerPath, // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
		FString assetName //Just the file name as displayed
	)
    */

    LoadAndSaveAssetToManager<weaponEnum, weaponAttachmentEnum, UClass>(
        weaponEnum::assaultRifle,      // track in asset manager
        weaponAttachmentEnum::reddot,  // track in asset manager
        "Game",                        // like "Game" for game or any other plugin name
        "Prefabs/Weapons/attachments", // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
        "reddotBp"                     // Just the file name as displayed
    );
    LoadAndSaveAssetToManager<weaponEnum, weaponAttachmentEnum, UClass>(
        weaponEnum::assaultRifle,         // track in asset manager
        weaponAttachmentEnum::iron_sight, // track in asset manager
        "Game",                           // like "Game" for game or any other plugin name
        "Prefabs/Weapons/rifle2",         // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
        "ironsightbp"                     // Just the file name as displayed
    );
    LoadAndSaveAssetToManager<weaponEnum, weaponAttachmentEnum, UClass>(
        weaponEnum::pistol,            // track in asset manager
        weaponAttachmentEnum::reddot,  // track in asset manager
        "Game",                        // like "Game" for game or any other plugin name
        "Prefabs/Weapons/attachments", // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
        "reddotPistolBp"               // Just the file name as displayed
    );


    //supressor 2 times...
    LoadAndSaveAssetToManager<weaponEnum, weaponAttachmentEnum, UClass>(
        weaponEnum::pistol,                           // track in asset manager
        weaponAttachmentEnum::muzzle_SoundSurpressor, // track in asset manager
        "Game",                                       // like "Game" for game or any other plugin name
        "Prefabs/Weapons/attachments",                // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
        "surpressorBp"                                // Just the file name as displayed
    );


    LoadAndSaveAssetToManager<weaponEnum, weaponAttachmentEnum, UClass>(
        weaponEnum::assaultRifle,                           // track in asset manager
        weaponAttachmentEnum::muzzle_SoundSurpressor, // track in asset manager
        "Game",                                       // like "Game" for game or any other plugin name
        "Prefabs/Weapons/attachments",                // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
        "surpressorBp"                                // Just the file name as displayed
    );

    //muendungs feuer daempfer 2 times

    LoadAndSaveAssetToManager<weaponEnum, weaponAttachmentEnum, UClass>(
        weaponEnum::pistol,                           // track in asset manager
        weaponAttachmentEnum::muzzle_flashSurpressor, // track in asset manager
        "Game",                                       // like "Game" for game or any other plugin name
        "Prefabs/Weapons/attachments",                // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
        "MundungsfeuerdampferBp"                      // Just the file name as displayed
    );
    LoadAndSaveAssetToManager<weaponEnum, weaponAttachmentEnum, UClass>(
        weaponEnum::assaultRifle,                           // track in asset manager
        weaponAttachmentEnum::muzzle_flashSurpressor, // track in asset manager
        "Game",                                       // like "Game" for game or any other plugin name
        "Prefabs/Weapons/attachments",                // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
        "MundungsfeuerdampferBp"                      // Just the file name as displayed
    );


}




/// @brief loads all weapons to the entity manager
void AssetLoader::loadWeapons(){

    LoadAndSaveAssetToManager<weaponEnum, UClass>(
        weaponEnum::pistol,                 // track in asset manager
        "Game",                             // like "Game" for game or any other plugin name
        "Prefabs/Weapons/pistol/pistolNew", // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
        "pistolNew"                         // Just the file name as displayed
    );

    LoadAndSaveAssetToManager<weaponEnum, UClass>(
        weaponEnum::assaultRifle,                 // track in asset manager
        "Game",                             // like "Game" for game or any other plugin name
        "Prefabs/Weapons/rifle2", // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
        "rifleBp"                         // Just the file name as displayed
    );


    LoadAndSaveAssetToManager<weaponEnum, UClass>(
        weaponEnum::thrower,            // track in asset manager
        "Game",                         // like "Game" for game or any other plugin name
        "Prefabs/Throwables",           // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
        "defaultthrower"                // Just the file name as displayed
    );
    


}


/// @brief load throwables
/// @param entityManager entity manager
void AssetLoader::loadThrowables(){
    
    LoadAndSaveAssetToManager<throwableEnum, UClass>(
        throwableEnum::greneade_enum, // track in asset manager
        "Game",                       // like "Game" for game or any other plugin name
        "Prefabs/Throwables",         // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
        "grenadeBp"                   // Just the file name as displayed
    );
    
    
}



/// @brief load particles for the entitymanager
/// @param entityManager to set in
void AssetLoader::loadParticles(){
    
    LoadAndSaveAssetToManager<particleEnum, UClass>(
        particleEnum::smoke_enum, // track in asset manager
        "Game",                       // like "Game" for game or any other plugin name
        "Prefabs/particle",         // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
        "particleSmoke"                   // Just the file name as displayed
    );

    LoadAndSaveAssetToManager<particleEnum, UClass>(
        particleEnum::fire_enum, // track in asset manager
        "Game",                  // like "Game" for game or any other plugin name
        "Prefabs/particle",      // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
        "particleFire"           // Just the file name as displayed
    );

    LoadAndSaveAssetToManager<particleEnum, UClass>(
        particleEnum::particleNone_enum, // track in asset manager
        "Game",                  // like "Game" for game or any other plugin name
        "Prefabs/particle",      // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
        "particleNone"           // Just the file name as displayed
    );
    



}



// ---- LOAD MATERIALS ----

/// @brief loads all materials needed
void AssetLoader::loadMaterials(){
    /*
    Default call:

    LoadAndSaveAssetToManager<materialEnum, UMaterial>(
        materialEnum::grassMaterial, 
        "Game", //plugin name or "Game" 
        "Prefabs/terrain/materials", //inner path, not trailing slash
        "grassMaterial" //asset name just as displayed
    );*/

    //CAUTION:
    //since im saving all materials in the same folder:
    std::map<materialEnum, FString> assetNames;
    assetNames[materialEnum::grassMaterial] = "grassMaterial";
    assetNames[materialEnum::stoneMaterial] = "stoneMaterial";
    assetNames[materialEnum::beigeStoneMaterial] = "beigeStoneMaterial";
    assetNames[materialEnum::wingMaterial] = "wingMaterial";
    assetNames[materialEnum::treeMaterial] = "treeMaterial";
    assetNames[materialEnum::palmLeafMaterial] = "palmLeafMaterial";
    assetNames[materialEnum::CactusMaterial] = "CactusMaterial";
    assetNames[materialEnum::sandMaterial] = "sandMaterial";
    assetNames[materialEnum::redsandMaterial] = "redsandMaterial";
    assetNames[materialEnum::waterMaterial] = "waterMaterial";

    assetNames[materialEnum::snowMaterial] = "snowMaterial";
    assetNames[materialEnum::_texturedMaterial] = "texturedMaterial";
    assetNames[materialEnum::prop_alarmBoxMaterial] = "prop_alarmBoxMaterial";
    assetNames[materialEnum::wallMaterial] = "wallMaterial";
    assetNames[materialEnum::glassMaterial] = "glassMaterial";

    for(auto &pair : assetNames){
        materialEnum typeEnum = pair.first;
        FString nameAsset = pair.second;
        LoadAndSaveAssetToManager<materialEnum, UMaterial>(
            typeEnum, 
            "Game", //plugin name or "Game" 
            "Prefabs/terrain/materials", //inner path, not trailing slash
            nameAsset //asset name just as displayed
        );
    }


}






void AssetLoader::loadTextures(){
    
    //icons directory
    std::map<textureEnum, FString> assetNames;
    assetNames[textureEnum::patroneIcon] = "patroneIcon";
    assetNames[textureEnum::healthIcon] = "healthIcon";
    assetNames[textureEnum::enemyMarkerIcon] = "enemyMarkerIcon";
    assetNames[textureEnum::playerMarkerIcon] = "playerMarkerIcon";

    for(auto &pair : assetNames){
        textureEnum typeEnum = pair.first;
        FString nameAsset = pair.second;
        LoadAndSaveAssetToManager<textureEnum, UTexture2D>(
            typeEnum, 
            "Game", //plugin name or "Game" 
            "Prefabs/icons", //inner path, not trailing slash
            nameAsset //asset name just as displayed
        );
    }
    
    
    
    
    
}





void AssetLoader::loadDebugCube(){
    if(assetManager *am = assetManager::instance()){

        FString path = FString::Printf(TEXT(
            "Blueprint'/Game/Prefabs/debug/debugCube.debugCube_C'"
        ));
        am->setDebugCubeBp(loadUClassBluePrint(path));
    }
}



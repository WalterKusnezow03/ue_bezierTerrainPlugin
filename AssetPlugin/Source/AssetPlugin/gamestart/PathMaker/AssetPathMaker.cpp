#include "AssetPathMaker.h"

AssetPathMaker::AssetPathMaker(){

}

AssetPathMaker::~AssetPathMaker(){

}



void AssetPathMaker::Test(){
    //Blueprint
    //TEXT("Blueprint'/Game/Prefabs/particle/particleNone.particleNone_C'"))

    //material
    //TEXT("Material'/Game/Prefabs/terrain/materials/treeMaterial.treeMaterial'")

    //texture2D
    //TEXT("Texture2D'/Game/Prefabs/icons/healthIcon.healthIcon'")

    AssetPathMaker maker;
    FString debugPath1 = maker.makeAssetPath(
        EAssetType::EUTexture2D,
        "Game",
        "Prefabs/particle/",
        "particleNone"
    );
    maker.logMessage(debugPath1);


    FString debugPath2 = maker.makeAssetPath(
        EAssetType::EUMaterial,
        "Game",
        "Prefabs/particle/",
        "particleNone"
    );
    maker.logMessage(debugPath2);


    FString debugPath3 = maker.makeAssetPath(
        EAssetType::EUClassBlueprint,
        "Game",
        "Prefabs/particle/",
        "particleNone"
    );
    maker.logMessage(debugPath3);
}

void AssetPathMaker::logMessage(FString message){
    FString prefix = FString::Printf(TEXT("AssetPathMaker: (%s)"), *message);
    UE_LOG(LogTemp, Log, TEXT("%s"), *prefix);
}




FString AssetPathMaker::makeGameAssetPath(
    EAssetType type,
    FString innerPath,
    FString assetName
){
    FString pluginName = TEXT("Game");
    return makeAssetPath(type, pluginName, innerPath, assetName);
}

FString AssetPathMaker::makeAssetPath(
    EAssetType type,
    FString pluginName,
    FString innerPath,
    FString assetName
){
    if(pluginName.Len() == 0){
        pluginName = TEXT("/Game");
    }
    AddBeginningSlashIfNeeded(pluginName);
    RemoveTrailingSlashIfNeeded(pluginName);

    innerPath = cleanInnerPath(innerPath);

    //Blueprint
    //TEXT("Blueprint'/Game/Prefabs/particle/particleNone.particleNone_C'"))

    //material
    //TEXT("Material'/Game/Prefabs/terrain/materials/treeMaterial.treeMaterial'")

    //texture2D
    //TEXT("Texture2D'/Game/Prefabs/icons/healthIcon.healthIcon'")

    FString assetTypePrefix = ToString(type);
    FString assetNamePostfix = AssetNamePostFix(type, assetName);

    FString path = FString::Printf(
        TEXT("%s'%s%s%s.%s'"),
        *assetTypePrefix,
        *pluginName,
        *innerPath,
        *assetName,
        *assetNamePostfix
    );
    return path;
}









FString AssetPathMaker::ToString(EAssetType type){
    if(type == EAssetType::EUClassBlueprint){
        return TEXT("Blueprint");
    }
    if(type == EAssetType::EUMaterial){
        return TEXT("Material");
    }
    if(type == EAssetType::EUTexture2D){
        return TEXT("Texture2D");
    }
    if(type == EAssetType::EUStaticMesh){
        return TEXT("StaticMesh");
    }

    return TEXT("none");
}

void AssetPathMaker::AddBeginningSlashIfNeeded(FString &pluginName){
    if(pluginName.Len() > 0){
        if(!isSlash(pluginName[0])){
            pluginName = TEXT("/") + pluginName;
        }
    }
}

void AssetPathMaker::RemoveTrailingSlashIfNeeded(FString &pluginName){
    if(pluginName.Len() > 0){
        int last = pluginName.Len() - 1;
        if (isSlash(pluginName[last]))
        {
            pluginName.RemoveAt(last);
        }
    }
}

FString AssetPathMaker::AssetNamePostFix(EAssetType type, FString assetName){
    if(type == EAssetType::EUClassBlueprint){
        return assetName + TEXT("_C");
    }
    return assetName;
}

/// @brief returns a format of "/somePath/ToAsset/", beginning and trailing slash. removes double slashes
/// @param path path to check
/// @return 
FString AssetPathMaker::cleanInnerPath(FString path){

    RemoveTrailingSlashIfNeeded(path);
    TArray<TCHAR> charArray = path.GetCharArray();
    TArray<TCHAR> validated;

    if(charArray.Num() == 0){
        return TEXT("/");
    }

    validated.Add('/'); //beginning slash
    for (int i = 0; i < charArray.Num(); i++){
        TCHAR &current = charArray[i];

        if(isSlash(current)){
            if(!latestIsSlash(validated)){
                validated.Add(current);
            }
        }else{
            validated.Add(current);
        }
    }
    if(!latestIsSlash(validated)){
        validated.Add('/'); //trailing slash
    }
    


    FString constructedString = TEXT("");
    for (int i = 0; i < validated.Num(); i++){
        constructedString += validated[i];
    }
    return constructedString;
}

bool AssetPathMaker::latestIsSlash(TArray<TCHAR> &text){
    if(text.Num() > 0){
        return isSlash(text[text.Num() - 1]);
    }
    return false;
}

bool AssetPathMaker::isSlash(TCHAR &character){
    return character == '/';
}
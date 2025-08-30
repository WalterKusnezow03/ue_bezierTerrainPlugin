#pragma once 
#include "CoreMinimal.h"
#include "AssetPlugin/gamestart/PathMaker/enum/EAssetType.h"

class ASSETPLUGIN_API AssetPathMaker {

public:
    AssetPathMaker();
    ~AssetPathMaker();

    static void Test();

    FString makeGameAssetPath(
        EAssetType type,
        FString innerPath,
        FString assetName
    );

    /// @brief 
    /// @param type type of asset
    /// @param pluginName plugin name, no slashes
    /// @param innerPath inner path, no starting or trailing slash needed
    /// @param assetName asset name, no slashes
    /// @return 
    FString makeAssetPath(
        EAssetType type,
        FString pluginName, //cant be empty
        FString innerPath, //can be empty!
        FString assetName
    );

private:
    FString ToString(EAssetType type);

    FString cleanInnerPath(FString path);
    bool latestIsSlash(TArray<TCHAR> &text);
    bool isSlash(TCHAR &character);

    void AddBeginningSlashIfNeeded(FString &pluginName);
    void RemoveTrailingSlashIfNeeded(FString &pluginName);

    FString AssetNamePostFix(EAssetType type, FString assetName);

    void logMessage(FString printing);
};
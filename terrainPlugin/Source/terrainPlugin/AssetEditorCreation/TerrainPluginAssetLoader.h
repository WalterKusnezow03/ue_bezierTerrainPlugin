#pragma once

#include "CoreMinimal.h"

class TERRAINPLUGIN_API TerrainPluginAssetLoader {

#if WITH_EDITOR
public:
    static void InitOnLaunchModule();
    static void LoadGrassAssetOnBeginPlay();

private:
    static void InitGrassAssets();
#endif
};
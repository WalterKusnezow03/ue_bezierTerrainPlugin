#pragma once

#include "CoreMinimal.h"

class TERRAINPLUGIN_API TerrainPluginAssetLoader {


public:
    static void InitOnLaunchModule();
    static void LoadGrassAssetOnBeginPlay();

private:
    static void InitGrassAssets();

};
#pragma once

#include "CoreMinimal.h"

struct TERRAINPLUGIN_API FTerrainChunkAttributes{
    int posX = 0; //posX and Y will be also the id
    int posY = 0;
    int height = 0;
    bool outpostFlagged = false;
    bool buildingFlagged = false;
};
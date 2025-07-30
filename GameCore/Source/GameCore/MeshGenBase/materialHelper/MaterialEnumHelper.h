#pragma once

#include "CoreMinimal.h"


class GAMECORE_API MaterialEnumHelper{

public:
    static std::vector<materialEnum> materialVector();

    static materialEnum groundMaterialFor(ETerrainType terraintype);
};
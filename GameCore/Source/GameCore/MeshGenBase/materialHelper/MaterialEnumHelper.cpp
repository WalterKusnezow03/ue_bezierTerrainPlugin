#include "MaterialEnumHelper.h"



std::vector<materialEnum> MaterialEnumHelper::materialVector(){
    std::vector<materialEnum> types = {
        materialEnum::grassMaterial,
        materialEnum::wallMaterial,
        materialEnum::glassMaterial,
        materialEnum::stoneMaterial,
        materialEnum::sandMaterial,
        materialEnum::redsandMaterial,
        materialEnum::treeMaterial,
        materialEnum::palmLeafMaterial,
        materialEnum::waterMaterial,
        materialEnum::snowMaterial,
        materialEnum::beigeStoneMaterial,
        materialEnum::prop_alarmBoxMaterial,
        materialEnum::_texturedMaterial,
        materialEnum::wingMaterial
    };
    return types;
}




materialEnum MaterialEnumHelper::groundMaterialFor(ETerrainType terraintype){
    if(terraintype == ETerrainType::EOcean){
        return materialEnum::sandMaterial;
    }
    if(terraintype == ETerrainType::EDesert){
        return materialEnum::redsandMaterial;
    }
    if(terraintype == ETerrainType::ESnowHill){
        return materialEnum::snowMaterial;
    }
    if(terraintype == ETerrainType::EDesertForest){
        return materialEnum::beigeStoneMaterial;
    }

    return materialEnum::grassMaterial;
}
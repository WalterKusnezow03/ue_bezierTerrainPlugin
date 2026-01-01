#include "MaterialEnumHelper.h"
#include <map>


std::pair<materialEnum, FString> MaterialEnumHelper::MakePair(materialEnum type, FString name){
    return std::pair<materialEnum, FString>(type, name);
}

TArray<std::pair<materialEnum, FString>> MaterialEnumHelper::GetMaterialPairs(){
    TArray<std::pair<materialEnum, FString>> map;
    map.Add(MakePair(materialEnum::grassMaterial, "grassMaterial"));
    map.Add(MakePair(materialEnum::wallMaterial, "wallMaterial"));
    map.Add(MakePair(materialEnum::glassMaterial, "glassMaterial"));
    map.Add(MakePair(materialEnum::stoneMaterial, "stoneMaterial"));
    map.Add(MakePair(materialEnum::sandMaterial, "sandMaterial"));
    map.Add(MakePair(materialEnum::redsandMaterial, "redsandMaterial"));
    map.Add(MakePair(materialEnum::treeMaterial, "treeMaterial"));
    map.Add(MakePair(materialEnum::palmLeafMaterial, "palmLeafMaterial"));
    
    map.Add(MakePair(materialEnum::waterMaterial, "waterMaterial"));
    map.Add(MakePair(materialEnum::snowMaterial, "snowMaterial"));
    map.Add(MakePair(materialEnum::beigeStoneMaterial, "beigeStoneMaterial"));

    map.Add(MakePair(materialEnum::_texturedMaterial, "_texturedMaterial"));
    map.Add(MakePair(materialEnum::prop_alarmBoxMaterial, "prop_alarmBoxMaterial"));

    map.Add(MakePair(materialEnum::wingMaterial, "wingMaterial"));
    map.Add(MakePair(materialEnum::CactusMaterial, "CactusMaterial"));
    map.Add(MakePair(materialEnum::grassMaterialWithShader, "grassMaterialWithShader"));

    map.Add(MakePair(materialEnum::widgetMaterial, "widgetMaterial"));
    
    return map;
}

materialEnum MaterialEnumHelper::toMaterialEnum(FString typein){
    TArray<std::pair<materialEnum, FString>> map = GetMaterialPairs();
    for (int i = 0; i < map.Num(); i++){
        std::pair<materialEnum, FString> &current = map[i];
        if(current.second == typein){
            return current.first;
        }
    }
    return materialEnum::grassMaterial; //default.
}

FString MaterialEnumHelper::toString(materialEnum typein){
    TArray<std::pair<materialEnum, FString>> map = GetMaterialPairs();
    for (int i = 0; i < map.Num(); i++){
        std::pair<materialEnum, FString> &current = map[i];
        if(current.first == typein){
            return current.second;
        }
    }

    return TEXT("materialNotFound");
}

std::vector<materialEnum> MaterialEnumHelper::materialVector(){
    TArray<std::pair<materialEnum, FString>> pairs = GetMaterialPairs();
    std::vector<materialEnum> types;
    for (int i = 0; i < pairs.Num(); i++){
        std::pair<materialEnum, FString> &current = pairs[i];
        types.push_back(current.first);
    }

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


int MaterialEnumHelper::indexFor(materialEnum type){
    int outIndex = 0;
    std::vector<materialEnum> allMaterials = materialVector();
    for (int i = 0; i < allMaterials.size(); i++){
        if(allMaterials[i] == type){
            return i;
        }
    }
    return outIndex;
}
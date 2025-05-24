// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "MeshGenBase/foliage/ETerrainType.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef GAMECORE_ETerrainType_generated_h
#error "ETerrainType.generated.h already included, missing '#pragma once' in ETerrainType.h"
#endif
#define GAMECORE_ETerrainType_generated_h

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_UnrealProjects_p2_Plugins_GameCore_Source_GameCore_MeshGenBase_foliage_ETerrainType_h


#define FOREACH_ENUM_ETERRAINTYPE(op) \
	op(ETerrainType::EForest) \
	op(ETerrainType::ETropical) \
	op(ETerrainType::EDesert) \
	op(ETerrainType::EDesertForest) \
	op(ETerrainType::EOcean) \
	op(ETerrainType::ESnowHill) 

enum class ETerrainType;
template<> struct TIsUEnumClass<ETerrainType> { enum { Value = true }; };
template<> GAMECORE_API UEnum* StaticEnum<ETerrainType>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS

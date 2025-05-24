// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "meshgen/foliage/ETreeType.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef TERRAINPLUGIN_ETreeType_generated_h
#error "ETreeType.generated.h already included, missing '#pragma once' in ETreeType.h"
#endif
#define TERRAINPLUGIN_ETreeType_generated_h

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_foliage_ETreeType_h


#define FOREACH_ENUM_ETREETYPE(op) \
	op(ETreeType::Edefault) \
	op(ETreeType::EPalmTree) \
	op(ETreeType::EPalmBush) \
	op(ETreeType::ECactus) 

enum class ETreeType;
template<> struct TIsUEnumClass<ETreeType> { enum { Value = true }; };
template<> TERRAINPLUGIN_API UEnum* StaticEnum<ETreeType>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS

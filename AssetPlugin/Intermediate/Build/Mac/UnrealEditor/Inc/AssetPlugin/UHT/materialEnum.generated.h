// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "gamestart/assetEnums/materialEnum.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef ASSETPLUGIN_materialEnum_generated_h
#error "materialEnum.generated.h already included, missing '#pragma once' in materialEnum.h"
#endif
#define ASSETPLUGIN_materialEnum_generated_h

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_assetEnums_materialEnum_h


#define FOREACH_ENUM_MATERIALENUM(op) \
	op(materialEnum::grassMaterial) \
	op(materialEnum::wallMaterial) \
	op(materialEnum::glassMaterial) \
	op(materialEnum::stoneMaterial) \
	op(materialEnum::beigeStoneMaterial) \
	op(materialEnum::sandMaterial) \
	op(materialEnum::redsandMaterial) \
	op(materialEnum::treeMaterial) \
	op(materialEnum::palmLeafMaterial) \
	op(materialEnum::waterMaterial) \
	op(materialEnum::snowMaterial) \
	op(materialEnum::prop_alarmBoxMaterial) \
	op(materialEnum::_texturedMaterial) \
	op(materialEnum::wingMaterial) 

enum class materialEnum;
template<> struct TIsUEnumClass<materialEnum> { enum { Value = true }; };
template<> ASSETPLUGIN_API UEnum* StaticEnum<materialEnum>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS

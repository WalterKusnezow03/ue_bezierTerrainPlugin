// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "gamestart/assetEnums/textureEnum.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef ASSETPLUGIN_textureEnum_generated_h
#error "textureEnum.generated.h already included, missing '#pragma once' in textureEnum.h"
#endif
#define ASSETPLUGIN_textureEnum_generated_h

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_assetEnums_textureEnum_h


#define FOREACH_ENUM_TEXTUREENUM(op) \
	op(textureEnum::patroneIcon) \
	op(textureEnum::healthIcon) \
	op(textureEnum::enemyMarkerIcon) \
	op(textureEnum::playerMarkerIcon) 

enum class textureEnum;
template<> struct TIsUEnumClass<textureEnum> { enum { Value = true }; };
template<> ASSETPLUGIN_API UEnum* StaticEnum<textureEnum>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS

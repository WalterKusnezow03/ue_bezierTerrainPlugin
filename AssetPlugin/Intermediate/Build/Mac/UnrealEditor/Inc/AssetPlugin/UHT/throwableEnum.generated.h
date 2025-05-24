// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "gamestart/assetEnums/throwableEnum.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef ASSETPLUGIN_throwableEnum_generated_h
#error "throwableEnum.generated.h already included, missing '#pragma once' in throwableEnum.h"
#endif
#define ASSETPLUGIN_throwableEnum_generated_h

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_assetEnums_throwableEnum_h


#define FOREACH_ENUM_THROWABLEENUM(op) \
	op(throwableEnum::rock_enum) \
	op(throwableEnum::greneade_enum) \
	op(throwableEnum::rocket_enum) \
	op(throwableEnum::molotov_enum) 

enum class throwableEnum;
template<> struct TIsUEnumClass<throwableEnum> { enum { Value = true }; };
template<> ASSETPLUGIN_API UEnum* StaticEnum<throwableEnum>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS

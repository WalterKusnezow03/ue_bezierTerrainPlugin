// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "gamestart/assetEnums/particleEnum.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef ASSETPLUGIN_particleEnum_generated_h
#error "particleEnum.generated.h already included, missing '#pragma once' in particleEnum.h"
#endif
#define ASSETPLUGIN_particleEnum_generated_h

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_assetEnums_particleEnum_h


#define FOREACH_ENUM_PARTICLEENUM(op) \
	op(particleEnum::particleNone_enum) \
	op(particleEnum::smoke_enum) \
	op(particleEnum::fire_enum) 

enum class particleEnum;
template<> struct TIsUEnumClass<particleEnum> { enum { Value = true }; };
template<> ASSETPLUGIN_API UEnum* StaticEnum<particleEnum>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS

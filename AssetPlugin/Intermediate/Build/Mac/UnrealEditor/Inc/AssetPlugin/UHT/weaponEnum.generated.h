// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "gamestart/assetEnums/weaponEnum.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef ASSETPLUGIN_weaponEnum_generated_h
#error "weaponEnum.generated.h already included, missing '#pragma once' in weaponEnum.h"
#endif
#define ASSETPLUGIN_weaponEnum_generated_h

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_assetEnums_weaponEnum_h


#define FOREACH_ENUM_WEAPONENUM(op) \
	op(weaponEnum::assaultRifle) \
	op(weaponEnum::pistol) \
	op(weaponEnum::thrower) 

enum class weaponEnum;
template<> struct TIsUEnumClass<weaponEnum> { enum { Value = true }; };
template<> ASSETPLUGIN_API UEnum* StaticEnum<weaponEnum>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS

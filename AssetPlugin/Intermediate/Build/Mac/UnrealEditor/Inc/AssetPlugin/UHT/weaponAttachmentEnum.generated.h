// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "gamestart/assetEnums/weaponAttachmentEnum.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef ASSETPLUGIN_weaponAttachmentEnum_generated_h
#error "weaponAttachmentEnum.generated.h already included, missing '#pragma once' in weaponAttachmentEnum.h"
#endif
#define ASSETPLUGIN_weaponAttachmentEnum_generated_h

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_assetEnums_weaponAttachmentEnum_h


#define FOREACH_ENUM_WEAPONATTACHMENTENUM(op) \
	op(weaponAttachmentEnum::iron_sight) \
	op(weaponAttachmentEnum::reddot) \
	op(weaponAttachmentEnum::grip_vertical) \
	op(weaponAttachmentEnum::muzzle_flashSurpressor) \
	op(weaponAttachmentEnum::muzzle_SoundSurpressor) 

enum class weaponAttachmentEnum;
template<> struct TIsUEnumClass<weaponAttachmentEnum> { enum { Value = true }; };
template<> ASSETPLUGIN_API UEnum* StaticEnum<weaponAttachmentEnum>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS

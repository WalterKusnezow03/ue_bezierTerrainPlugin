// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "gamestart/assetEnums/entityEnum.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef ASSETPLUGIN_entityEnum_generated_h
#error "entityEnum.generated.h already included, missing '#pragma once' in entityEnum.h"
#endif
#define ASSETPLUGIN_entityEnum_generated_h

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_assetEnums_entityEnum_h


#define FOREACH_ENUM_ENTITYENUM(op) \
	op(entityEnum::human_enum) \
	op(entityEnum::entity_enum) 

enum class entityEnum;
template<> struct TIsUEnumClass<entityEnum> { enum { Value = true }; };
template<> ASSETPLUGIN_API UEnum* StaticEnum<entityEnum>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS

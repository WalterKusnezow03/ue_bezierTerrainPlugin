// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "meshgen/rooms/roomtypeEnum.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef TERRAINPLUGIN_roomtypeEnum_generated_h
#error "roomtypeEnum.generated.h already included, missing '#pragma once' in roomtypeEnum.h"
#endif
#define TERRAINPLUGIN_roomtypeEnum_generated_h

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_rooms_roomtypeEnum_h


#define FOREACH_ENUM_ROOMTYPEENUM(op) \
	op(roomtypeEnum::room) \
	op(roomtypeEnum::staircase) \
	op(roomtypeEnum::staircaseTopper) 

enum class roomtypeEnum;
template<> struct TIsUEnumClass<roomtypeEnum> { enum { Value = true }; };
template<> TERRAINPLUGIN_API UEnum* StaticEnum<roomtypeEnum>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS

// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "EntityGC/trackedActors.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef GAMECORE_trackedActors_generated_h
#error "trackedActors.generated.h already included, missing '#pragma once' in trackedActors.h"
#endif
#define GAMECORE_trackedActors_generated_h

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_UnrealProjects_p2_Plugins_GameCore_Source_GameCore_EntityGC_trackedActors_h


#define FOREACH_ENUM_ETRACKEDACTORS(op) \
	op(ETrackedActors::EMeshActor) 

enum class ETrackedActors;
template<> struct TIsUEnumClass<ETrackedActors> { enum { Value = true }; };
template<> GAMECORE_API UEnum* StaticEnum<ETrackedActors>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS

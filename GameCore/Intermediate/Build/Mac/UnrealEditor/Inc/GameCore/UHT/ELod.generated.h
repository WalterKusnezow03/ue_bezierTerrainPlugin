// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "MeshGenBase/ELod.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef GAMECORE_ELod_generated_h
#error "ELod.generated.h already included, missing '#pragma once' in ELod.h"
#endif
#define GAMECORE_ELod_generated_h

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_UnrealProjects_p2_Plugins_GameCore_Source_GameCore_MeshGenBase_ELod_h


#define FOREACH_ENUM_ELOD(op) \
	op(ELod::lodNear) \
	op(ELod::lodMiddle) \
	op(ELod::lodFar) 

enum class ELod;
template<> struct TIsUEnumClass<ELod> { enum { Value = true }; };
template<> GAMECORE_API UEnum* StaticEnum<ELod>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS

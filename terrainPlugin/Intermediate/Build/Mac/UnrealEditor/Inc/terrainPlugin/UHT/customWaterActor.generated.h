// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "meshgen/water/customWaterActor.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef TERRAINPLUGIN_customWaterActor_generated_h
#error "customWaterActor.generated.h already included, missing '#pragma once' in customWaterActor.h"
#endif
#define TERRAINPLUGIN_customWaterActor_generated_h

#define FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_water_customWaterActor_h_17_SPARSE_DATA
#define FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_water_customWaterActor_h_17_RPC_WRAPPERS
#define FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_water_customWaterActor_h_17_RPC_WRAPPERS_NO_PURE_DECLS
#define FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_water_customWaterActor_h_17_ACCESSORS
#define FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_water_customWaterActor_h_17_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAcustomWaterActor(); \
	friend struct Z_Construct_UClass_AcustomWaterActor_Statics; \
public: \
	DECLARE_CLASS(AcustomWaterActor, AcustomMeshActorBase, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/terrainPlugin"), NO_API) \
	DECLARE_SERIALIZER(AcustomWaterActor) \
	virtual UObject* _getUObject() const override { return const_cast<AcustomWaterActor*>(this); }


#define FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_water_customWaterActor_h_17_INCLASS \
private: \
	static void StaticRegisterNativesAcustomWaterActor(); \
	friend struct Z_Construct_UClass_AcustomWaterActor_Statics; \
public: \
	DECLARE_CLASS(AcustomWaterActor, AcustomMeshActorBase, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/terrainPlugin"), NO_API) \
	DECLARE_SERIALIZER(AcustomWaterActor) \
	virtual UObject* _getUObject() const override { return const_cast<AcustomWaterActor*>(this); }


#define FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_water_customWaterActor_h_17_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API AcustomWaterActor(const FObjectInitializer& ObjectInitializer); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(AcustomWaterActor) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AcustomWaterActor); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AcustomWaterActor); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API AcustomWaterActor(AcustomWaterActor&&); \
	NO_API AcustomWaterActor(const AcustomWaterActor&); \
public: \
	NO_API virtual ~AcustomWaterActor();


#define FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_water_customWaterActor_h_17_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API AcustomWaterActor(AcustomWaterActor&&); \
	NO_API AcustomWaterActor(const AcustomWaterActor&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AcustomWaterActor); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AcustomWaterActor); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(AcustomWaterActor) \
	NO_API virtual ~AcustomWaterActor();


#define FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_water_customWaterActor_h_14_PROLOG
#define FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_water_customWaterActor_h_17_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_water_customWaterActor_h_17_SPARSE_DATA \
	FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_water_customWaterActor_h_17_RPC_WRAPPERS \
	FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_water_customWaterActor_h_17_ACCESSORS \
	FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_water_customWaterActor_h_17_INCLASS \
	FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_water_customWaterActor_h_17_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_water_customWaterActor_h_17_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_water_customWaterActor_h_17_SPARSE_DATA \
	FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_water_customWaterActor_h_17_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_water_customWaterActor_h_17_ACCESSORS \
	FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_water_customWaterActor_h_17_INCLASS_NO_PURE_DECLS \
	FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_water_customWaterActor_h_17_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> TERRAINPLUGIN_API UClass* StaticClass<class AcustomWaterActor>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_water_customWaterActor_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS

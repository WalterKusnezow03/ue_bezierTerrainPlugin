// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "terrainPlugin/main/TerrainLauncher.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeTerrainLauncher() {}
// Cross Module References
	ENGINE_API UClass* Z_Construct_UClass_AActor();
	TERRAINPLUGIN_API UClass* Z_Construct_UClass_ATerrainLauncher();
	TERRAINPLUGIN_API UClass* Z_Construct_UClass_ATerrainLauncher_NoRegister();
	UPackage* Z_Construct_UPackage__Script_terrainPlugin();
// End Cross Module References
	void ATerrainLauncher::StaticRegisterNativesATerrainLauncher()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(ATerrainLauncher);
	UClass* Z_Construct_UClass_ATerrainLauncher_NoRegister()
	{
		return ATerrainLauncher::StaticClass();
	}
	struct Z_Construct_UClass_ATerrainLauncher_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ATerrainLauncher_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AActor,
		(UObject* (*)())Z_Construct_UPackage__Script_terrainPlugin,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ATerrainLauncher_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/// @brief will launch the terrain generation or try to load from save storage\n" },
		{ "IncludePath", "main/TerrainLauncher.h" },
		{ "ModuleRelativePath", "main/TerrainLauncher.h" },
		{ "ToolTip", "@brief will launch the terrain generation or try to load from save storage" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_ATerrainLauncher_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ATerrainLauncher>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_ATerrainLauncher_Statics::ClassParams = {
		&ATerrainLauncher::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x009000A4u,
		METADATA_PARAMS(Z_Construct_UClass_ATerrainLauncher_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_ATerrainLauncher_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_ATerrainLauncher()
	{
		if (!Z_Registration_Info_UClass_ATerrainLauncher.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ATerrainLauncher.OuterSingleton, Z_Construct_UClass_ATerrainLauncher_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_ATerrainLauncher.OuterSingleton;
	}
	template<> TERRAINPLUGIN_API UClass* StaticClass<ATerrainLauncher>()
	{
		return ATerrainLauncher::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(ATerrainLauncher);
	ATerrainLauncher::~ATerrainLauncher() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_main_TerrainLauncher_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_main_TerrainLauncher_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_ATerrainLauncher, ATerrainLauncher::StaticClass, TEXT("ATerrainLauncher"), &Z_Registration_Info_UClass_ATerrainLauncher, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ATerrainLauncher), 245781324U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_main_TerrainLauncher_h_3556037485(TEXT("/Script/terrainPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_main_TerrainLauncher_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_main_TerrainLauncher_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS

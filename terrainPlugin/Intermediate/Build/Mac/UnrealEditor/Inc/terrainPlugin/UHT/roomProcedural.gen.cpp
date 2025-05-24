// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "terrainPlugin/meshgen/rooms/roomActor/roomProcedural.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCoderoomProcedural() {}
// Cross Module References
	TERRAINPLUGIN_API UClass* Z_Construct_UClass_AcustomMeshActor();
	TERRAINPLUGIN_API UClass* Z_Construct_UClass_AroomProcedural();
	TERRAINPLUGIN_API UClass* Z_Construct_UClass_AroomProcedural_NoRegister();
	UPackage* Z_Construct_UPackage__Script_terrainPlugin();
// End Cross Module References
	void AroomProcedural::StaticRegisterNativesAroomProcedural()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(AroomProcedural);
	UClass* Z_Construct_UClass_AroomProcedural_NoRegister()
	{
		return AroomProcedural::StaticClass();
	}
	struct Z_Construct_UClass_AroomProcedural_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AroomProcedural_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AcustomMeshActor,
		(UObject* (*)())Z_Construct_UPackage__Script_terrainPlugin,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AroomProcedural_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "meshgen/rooms/roomActor/roomProcedural.h" },
		{ "ModuleRelativePath", "meshgen/rooms/roomActor/roomProcedural.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_AroomProcedural_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AroomProcedural>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_AroomProcedural_Statics::ClassParams = {
		&AroomProcedural::StaticClass,
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
		METADATA_PARAMS(Z_Construct_UClass_AroomProcedural_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_AroomProcedural_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_AroomProcedural()
	{
		if (!Z_Registration_Info_UClass_AroomProcedural.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AroomProcedural.OuterSingleton, Z_Construct_UClass_AroomProcedural_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_AroomProcedural.OuterSingleton;
	}
	template<> TERRAINPLUGIN_API UClass* StaticClass<AroomProcedural>()
	{
		return AroomProcedural::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(AroomProcedural);
	AroomProcedural::~AroomProcedural() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_rooms_roomActor_roomProcedural_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_rooms_roomActor_roomProcedural_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_AroomProcedural, AroomProcedural::StaticClass, TEXT("AroomProcedural"), &Z_Registration_Info_UClass_AroomProcedural, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AroomProcedural), 1191219586U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_rooms_roomActor_roomProcedural_h_2512998102(TEXT("/Script/terrainPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_rooms_roomActor_roomProcedural_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_rooms_roomActor_roomProcedural_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS

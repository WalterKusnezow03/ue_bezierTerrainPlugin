// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "terrainPlugin/meshgen/customMeshActor.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodecustomMeshActor() {}
// Cross Module References
	GAMECORE_API UClass* Z_Construct_UClass_AcustomMeshActorBase();
	GAMECORE_API UClass* Z_Construct_UClass_UDamageinterface_NoRegister();
	TERRAINPLUGIN_API UClass* Z_Construct_UClass_AcustomMeshActor();
	TERRAINPLUGIN_API UClass* Z_Construct_UClass_AcustomMeshActor_NoRegister();
	UPackage* Z_Construct_UPackage__Script_terrainPlugin();
// End Cross Module References
	void AcustomMeshActor::StaticRegisterNativesAcustomMeshActor()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(AcustomMeshActor);
	UClass* Z_Construct_UClass_AcustomMeshActor_NoRegister()
	{
		return AcustomMeshActor::StaticClass();
	}
	struct Z_Construct_UClass_AcustomMeshActor_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const UECodeGen_Private::FImplementedInterfaceParams InterfaceParams[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AcustomMeshActor_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AcustomMeshActorBase,
		(UObject* (*)())Z_Construct_UPackage__Script_terrainPlugin,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AcustomMeshActor_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "meshgen/customMeshActor.h" },
		{ "ModuleRelativePath", "meshgen/customMeshActor.h" },
	};
#endif
		const UECodeGen_Private::FImplementedInterfaceParams Z_Construct_UClass_AcustomMeshActor_Statics::InterfaceParams[] = {
			{ Z_Construct_UClass_UDamageinterface_NoRegister, (int32)VTABLE_OFFSET(AcustomMeshActor, IDamageinterface), false },  // 2638534275
		};
	const FCppClassTypeInfoStatic Z_Construct_UClass_AcustomMeshActor_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AcustomMeshActor>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_AcustomMeshActor_Statics::ClassParams = {
		&AcustomMeshActor::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		InterfaceParams,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		UE_ARRAY_COUNT(InterfaceParams),
		0x009000A4u,
		METADATA_PARAMS(Z_Construct_UClass_AcustomMeshActor_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_AcustomMeshActor_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_AcustomMeshActor()
	{
		if (!Z_Registration_Info_UClass_AcustomMeshActor.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AcustomMeshActor.OuterSingleton, Z_Construct_UClass_AcustomMeshActor_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_AcustomMeshActor.OuterSingleton;
	}
	template<> TERRAINPLUGIN_API UClass* StaticClass<AcustomMeshActor>()
	{
		return AcustomMeshActor::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(AcustomMeshActor);
	AcustomMeshActor::~AcustomMeshActor() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_customMeshActor_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_customMeshActor_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_AcustomMeshActor, AcustomMeshActor::StaticClass, TEXT("AcustomMeshActor"), &Z_Registration_Info_UClass_AcustomMeshActor, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AcustomMeshActor), 3794178636U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_customMeshActor_h_534868452(TEXT("/Script/terrainPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_customMeshActor_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_customMeshActor_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS

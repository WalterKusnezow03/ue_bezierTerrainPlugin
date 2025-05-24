// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "terrainPlugin/meshgen/water/customWaterActor.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodecustomWaterActor() {}
// Cross Module References
	GAMECORE_API UClass* Z_Construct_UClass_AcustomMeshActorBase();
	GAMECORE_API UClass* Z_Construct_UClass_UDamageinterface_NoRegister();
	TERRAINPLUGIN_API UClass* Z_Construct_UClass_AcustomWaterActor();
	TERRAINPLUGIN_API UClass* Z_Construct_UClass_AcustomWaterActor_NoRegister();
	UPackage* Z_Construct_UPackage__Script_terrainPlugin();
// End Cross Module References
	void AcustomWaterActor::StaticRegisterNativesAcustomWaterActor()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(AcustomWaterActor);
	UClass* Z_Construct_UClass_AcustomWaterActor_NoRegister()
	{
		return AcustomWaterActor::StaticClass();
	}
	struct Z_Construct_UClass_AcustomWaterActor_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const UECodeGen_Private::FImplementedInterfaceParams InterfaceParams[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AcustomWaterActor_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AcustomMeshActorBase,
		(UObject* (*)())Z_Construct_UPackage__Script_terrainPlugin,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AcustomWaterActor_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "IncludePath", "meshgen/water/customWaterActor.h" },
		{ "ModuleRelativePath", "meshgen/water/customWaterActor.h" },
	};
#endif
		const UECodeGen_Private::FImplementedInterfaceParams Z_Construct_UClass_AcustomWaterActor_Statics::InterfaceParams[] = {
			{ Z_Construct_UClass_UDamageinterface_NoRegister, (int32)VTABLE_OFFSET(AcustomWaterActor, IDamageinterface), false },  // 2638534275
		};
	const FCppClassTypeInfoStatic Z_Construct_UClass_AcustomWaterActor_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AcustomWaterActor>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_AcustomWaterActor_Statics::ClassParams = {
		&AcustomWaterActor::StaticClass,
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
		METADATA_PARAMS(Z_Construct_UClass_AcustomWaterActor_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_AcustomWaterActor_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_AcustomWaterActor()
	{
		if (!Z_Registration_Info_UClass_AcustomWaterActor.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AcustomWaterActor.OuterSingleton, Z_Construct_UClass_AcustomWaterActor_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_AcustomWaterActor.OuterSingleton;
	}
	template<> TERRAINPLUGIN_API UClass* StaticClass<AcustomWaterActor>()
	{
		return AcustomWaterActor::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(AcustomWaterActor);
	AcustomWaterActor::~AcustomWaterActor() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_water_customWaterActor_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_water_customWaterActor_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_AcustomWaterActor, AcustomWaterActor::StaticClass, TEXT("AcustomWaterActor"), &Z_Registration_Info_UClass_AcustomWaterActor, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AcustomWaterActor), 1507610569U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_water_customWaterActor_h_1085907127(TEXT("/Script/terrainPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_water_customWaterActor_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_water_customWaterActor_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS

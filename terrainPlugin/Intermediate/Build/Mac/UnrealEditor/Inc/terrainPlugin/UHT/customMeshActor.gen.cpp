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
	TERRAINPLUGIN_API UClass* Z_Construct_UClass_AcustomMeshActor();
	TERRAINPLUGIN_API UClass* Z_Construct_UClass_AcustomMeshActor_NoRegister();
	TERRAINPLUGIN_API UClass* Z_Construct_UClass_UMovingFoliageInstancerComponent_NoRegister();
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
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_grassInstancer_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_grassInstancer;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
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
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AcustomMeshActor_Statics::NewProp_grassInstancer_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "meshgen/customMeshActor.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AcustomMeshActor_Statics::NewProp_grassInstancer = { "grassInstancer", nullptr, (EPropertyFlags)0x0020080000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(AcustomMeshActor, grassInstancer), Z_Construct_UClass_UMovingFoliageInstancerComponent_NoRegister, METADATA_PARAMS(Z_Construct_UClass_AcustomMeshActor_Statics::NewProp_grassInstancer_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_AcustomMeshActor_Statics::NewProp_grassInstancer_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_AcustomMeshActor_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AcustomMeshActor_Statics::NewProp_grassInstancer,
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
		Z_Construct_UClass_AcustomMeshActor_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_AcustomMeshActor_Statics::PropPointers),
		0,
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
		{ Z_Construct_UClass_AcustomMeshActor, AcustomMeshActor::StaticClass, TEXT("AcustomMeshActor"), &Z_Registration_Info_UClass_AcustomMeshActor, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AcustomMeshActor), 1631744609U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_customMeshActor_h_114697846(TEXT("/Script/terrainPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_customMeshActor_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_customMeshActor_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS

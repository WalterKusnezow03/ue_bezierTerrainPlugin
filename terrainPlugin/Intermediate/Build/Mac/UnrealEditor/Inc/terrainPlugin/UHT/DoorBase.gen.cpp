// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "terrainPlugin/meshgen/rooms/doorLike/DoorBase.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeDoorBase() {}
// Cross Module References
	GAMECORE_API UClass* Z_Construct_UClass_UInteractinterface_NoRegister();
	TERRAINPLUGIN_API UClass* Z_Construct_UClass_AcustomMeshActor();
	TERRAINPLUGIN_API UClass* Z_Construct_UClass_ADoorBase();
	TERRAINPLUGIN_API UClass* Z_Construct_UClass_ADoorBase_NoRegister();
	UPackage* Z_Construct_UPackage__Script_terrainPlugin();
// End Cross Module References
	void ADoorBase::StaticRegisterNativesADoorBase()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(ADoorBase);
	UClass* Z_Construct_UClass_ADoorBase_NoRegister()
	{
		return ADoorBase::StaticClass();
	}
	struct Z_Construct_UClass_ADoorBase_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const UECodeGen_Private::FImplementedInterfaceParams InterfaceParams[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ADoorBase_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AcustomMeshActor,
		(UObject* (*)())Z_Construct_UPackage__Script_terrainPlugin,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ADoorBase_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "meshgen/rooms/doorLike/DoorBase.h" },
		{ "ModuleRelativePath", "meshgen/rooms/doorLike/DoorBase.h" },
	};
#endif
		const UECodeGen_Private::FImplementedInterfaceParams Z_Construct_UClass_ADoorBase_Statics::InterfaceParams[] = {
			{ Z_Construct_UClass_UInteractinterface_NoRegister, (int32)VTABLE_OFFSET(ADoorBase, IInteractinterface), false },  // 1835687221
		};
	const FCppClassTypeInfoStatic Z_Construct_UClass_ADoorBase_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ADoorBase>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_ADoorBase_Statics::ClassParams = {
		&ADoorBase::StaticClass,
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
		METADATA_PARAMS(Z_Construct_UClass_ADoorBase_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_ADoorBase_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_ADoorBase()
	{
		if (!Z_Registration_Info_UClass_ADoorBase.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ADoorBase.OuterSingleton, Z_Construct_UClass_ADoorBase_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_ADoorBase.OuterSingleton;
	}
	template<> TERRAINPLUGIN_API UClass* StaticClass<ADoorBase>()
	{
		return ADoorBase::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(ADoorBase);
	ADoorBase::~ADoorBase() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_rooms_doorLike_DoorBase_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_rooms_doorLike_DoorBase_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_ADoorBase, ADoorBase::StaticClass, TEXT("ADoorBase"), &Z_Registration_Info_UClass_ADoorBase, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ADoorBase), 3079739705U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_rooms_doorLike_DoorBase_h_68490398(TEXT("/Script/terrainPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_rooms_doorLike_DoorBase_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_rooms_doorLike_DoorBase_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS

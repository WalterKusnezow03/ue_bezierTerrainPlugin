// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "GameCore/interfaces/Steeringinterface.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeSteeringinterface() {}
// Cross Module References
	COREUOBJECT_API UClass* Z_Construct_UClass_UInterface();
	GAMECORE_API UClass* Z_Construct_UClass_USteeringinterface();
	GAMECORE_API UClass* Z_Construct_UClass_USteeringinterface_NoRegister();
	UPackage* Z_Construct_UPackage__Script_GameCore();
// End Cross Module References
	void USteeringinterface::StaticRegisterNativesUSteeringinterface()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(USteeringinterface);
	UClass* Z_Construct_UClass_USteeringinterface_NoRegister()
	{
		return USteeringinterface::StaticClass();
	}
	struct Z_Construct_UClass_USteeringinterface_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_USteeringinterface_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UInterface,
		(UObject* (*)())Z_Construct_UPackage__Script_GameCore,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_USteeringinterface_Statics::Class_MetaDataParams[] = {
		{ "ModuleRelativePath", "interfaces/Steeringinterface.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_USteeringinterface_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ISteeringinterface>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_USteeringinterface_Statics::ClassParams = {
		&USteeringinterface::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x000840A1u,
		METADATA_PARAMS(Z_Construct_UClass_USteeringinterface_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_USteeringinterface_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_USteeringinterface()
	{
		if (!Z_Registration_Info_UClass_USteeringinterface.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_USteeringinterface.OuterSingleton, Z_Construct_UClass_USteeringinterface_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_USteeringinterface.OuterSingleton;
	}
	template<> GAMECORE_API UClass* StaticClass<USteeringinterface>()
	{
		return USteeringinterface::StaticClass();
	}
	USteeringinterface::USteeringinterface(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(USteeringinterface);
	USteeringinterface::~USteeringinterface() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_GameCore_Source_GameCore_interfaces_Steeringinterface_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_GameCore_Source_GameCore_interfaces_Steeringinterface_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_USteeringinterface, USteeringinterface::StaticClass, TEXT("USteeringinterface"), &Z_Registration_Info_UClass_USteeringinterface, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(USteeringinterface), 3367498803U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_GameCore_Source_GameCore_interfaces_Steeringinterface_h_2103320182(TEXT("/Script/GameCore"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_GameCore_Source_GameCore_interfaces_Steeringinterface_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_GameCore_Source_GameCore_interfaces_Steeringinterface_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS

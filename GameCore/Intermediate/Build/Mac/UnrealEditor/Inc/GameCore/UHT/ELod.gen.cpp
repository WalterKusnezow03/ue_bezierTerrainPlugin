// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "GameCore/MeshGenBase/ELod.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeELod() {}
// Cross Module References
	GAMECORE_API UEnum* Z_Construct_UEnum_GameCore_ELod();
	UPackage* Z_Construct_UPackage__Script_GameCore();
// End Cross Module References
	static FEnumRegistrationInfo Z_Registration_Info_UEnum_ELod;
	static UEnum* ELod_StaticEnum()
	{
		if (!Z_Registration_Info_UEnum_ELod.OuterSingleton)
		{
			Z_Registration_Info_UEnum_ELod.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_GameCore_ELod, (UObject*)Z_Construct_UPackage__Script_GameCore(), TEXT("ELod"));
		}
		return Z_Registration_Info_UEnum_ELod.OuterSingleton;
	}
	template<> GAMECORE_API UEnum* StaticEnum<ELod>()
	{
		return ELod_StaticEnum();
	}
	struct Z_Construct_UEnum_GameCore_ELod_Statics
	{
		static const UECodeGen_Private::FEnumeratorParam Enumerators[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[];
#endif
		static const UECodeGen_Private::FEnumParams EnumParams;
	};
	const UECodeGen_Private::FEnumeratorParam Z_Construct_UEnum_GameCore_ELod_Statics::Enumerators[] = {
		{ "ELod::lodNear", (int64)ELod::lodNear },
		{ "ELod::lodFar", (int64)ELod::lodFar },
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UEnum_GameCore_ELod_Statics::Enum_MetaDataParams[] = {
		{ "lodFar.Name", "ELod::lodFar" },
		{ "lodNear.Name", "ELod::lodNear" },
		{ "ModuleRelativePath", "MeshGenBase/ELod.h" },
	};
#endif
	const UECodeGen_Private::FEnumParams Z_Construct_UEnum_GameCore_ELod_Statics::EnumParams = {
		(UObject*(*)())Z_Construct_UPackage__Script_GameCore,
		nullptr,
		"ELod",
		"ELod",
		Z_Construct_UEnum_GameCore_ELod_Statics::Enumerators,
		UE_ARRAY_COUNT(Z_Construct_UEnum_GameCore_ELod_Statics::Enumerators),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EEnumFlags::None,
		(uint8)UEnum::ECppForm::EnumClass,
		METADATA_PARAMS(Z_Construct_UEnum_GameCore_ELod_Statics::Enum_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UEnum_GameCore_ELod_Statics::Enum_MetaDataParams))
	};
	UEnum* Z_Construct_UEnum_GameCore_ELod()
	{
		if (!Z_Registration_Info_UEnum_ELod.InnerSingleton)
		{
			UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_ELod.InnerSingleton, Z_Construct_UEnum_GameCore_ELod_Statics::EnumParams);
		}
		return Z_Registration_Info_UEnum_ELod.InnerSingleton;
	}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_GameCore_Source_GameCore_MeshGenBase_ELod_h_Statics
	{
		static const FEnumRegisterCompiledInInfo EnumInfo[];
	};
	const FEnumRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_GameCore_Source_GameCore_MeshGenBase_ELod_h_Statics::EnumInfo[] = {
		{ ELod_StaticEnum, TEXT("ELod"), &Z_Registration_Info_UEnum_ELod, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 3079056863U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_GameCore_Source_GameCore_MeshGenBase_ELod_h_859387015(TEXT("/Script/GameCore"),
		nullptr, 0,
		nullptr, 0,
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_GameCore_Source_GameCore_MeshGenBase_ELod_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_GameCore_Source_GameCore_MeshGenBase_ELod_h_Statics::EnumInfo));
PRAGMA_ENABLE_DEPRECATION_WARNINGS

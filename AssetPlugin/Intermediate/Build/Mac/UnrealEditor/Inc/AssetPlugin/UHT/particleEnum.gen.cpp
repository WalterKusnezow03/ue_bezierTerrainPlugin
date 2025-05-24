// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "AssetPlugin/gamestart/assetEnums/particleEnum.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeparticleEnum() {}
// Cross Module References
	ASSETPLUGIN_API UEnum* Z_Construct_UEnum_AssetPlugin_particleEnum();
	UPackage* Z_Construct_UPackage__Script_AssetPlugin();
// End Cross Module References
	static FEnumRegistrationInfo Z_Registration_Info_UEnum_particleEnum;
	static UEnum* particleEnum_StaticEnum()
	{
		if (!Z_Registration_Info_UEnum_particleEnum.OuterSingleton)
		{
			Z_Registration_Info_UEnum_particleEnum.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_AssetPlugin_particleEnum, (UObject*)Z_Construct_UPackage__Script_AssetPlugin(), TEXT("particleEnum"));
		}
		return Z_Registration_Info_UEnum_particleEnum.OuterSingleton;
	}
	template<> ASSETPLUGIN_API UEnum* StaticEnum<particleEnum>()
	{
		return particleEnum_StaticEnum();
	}
	struct Z_Construct_UEnum_AssetPlugin_particleEnum_Statics
	{
		static const UECodeGen_Private::FEnumeratorParam Enumerators[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[];
#endif
		static const UECodeGen_Private::FEnumParams EnumParams;
	};
	const UECodeGen_Private::FEnumeratorParam Z_Construct_UEnum_AssetPlugin_particleEnum_Statics::Enumerators[] = {
		{ "particleEnum::particleNone_enum", (int64)particleEnum::particleNone_enum },
		{ "particleEnum::smoke_enum", (int64)particleEnum::smoke_enum },
		{ "particleEnum::fire_enum", (int64)particleEnum::fire_enum },
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UEnum_AssetPlugin_particleEnum_Statics::Enum_MetaDataParams[] = {
		{ "fire_enum.Name", "particleEnum::fire_enum" },
		{ "ModuleRelativePath", "gamestart/assetEnums/particleEnum.h" },
		{ "particleNone_enum.Name", "particleEnum::particleNone_enum" },
		{ "smoke_enum.Name", "particleEnum::smoke_enum" },
	};
#endif
	const UECodeGen_Private::FEnumParams Z_Construct_UEnum_AssetPlugin_particleEnum_Statics::EnumParams = {
		(UObject*(*)())Z_Construct_UPackage__Script_AssetPlugin,
		nullptr,
		"particleEnum",
		"particleEnum",
		Z_Construct_UEnum_AssetPlugin_particleEnum_Statics::Enumerators,
		UE_ARRAY_COUNT(Z_Construct_UEnum_AssetPlugin_particleEnum_Statics::Enumerators),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EEnumFlags::None,
		(uint8)UEnum::ECppForm::EnumClass,
		METADATA_PARAMS(Z_Construct_UEnum_AssetPlugin_particleEnum_Statics::Enum_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UEnum_AssetPlugin_particleEnum_Statics::Enum_MetaDataParams))
	};
	UEnum* Z_Construct_UEnum_AssetPlugin_particleEnum()
	{
		if (!Z_Registration_Info_UEnum_particleEnum.InnerSingleton)
		{
			UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_particleEnum.InnerSingleton, Z_Construct_UEnum_AssetPlugin_particleEnum_Statics::EnumParams);
		}
		return Z_Registration_Info_UEnum_particleEnum.InnerSingleton;
	}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_assetEnums_particleEnum_h_Statics
	{
		static const FEnumRegisterCompiledInInfo EnumInfo[];
	};
	const FEnumRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_assetEnums_particleEnum_h_Statics::EnumInfo[] = {
		{ particleEnum_StaticEnum, TEXT("particleEnum"), &Z_Registration_Info_UEnum_particleEnum, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 1520216197U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_assetEnums_particleEnum_h_4099963473(TEXT("/Script/AssetPlugin"),
		nullptr, 0,
		nullptr, 0,
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_assetEnums_particleEnum_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_assetEnums_particleEnum_h_Statics::EnumInfo));
PRAGMA_ENABLE_DEPRECATION_WARNINGS

// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "AssetPlugin/gamestart/assetEnums/materialEnum.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodematerialEnum() {}
// Cross Module References
	ASSETPLUGIN_API UEnum* Z_Construct_UEnum_AssetPlugin_materialEnum();
	UPackage* Z_Construct_UPackage__Script_AssetPlugin();
// End Cross Module References
	static FEnumRegistrationInfo Z_Registration_Info_UEnum_materialEnum;
	static UEnum* materialEnum_StaticEnum()
	{
		if (!Z_Registration_Info_UEnum_materialEnum.OuterSingleton)
		{
			Z_Registration_Info_UEnum_materialEnum.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_AssetPlugin_materialEnum, (UObject*)Z_Construct_UPackage__Script_AssetPlugin(), TEXT("materialEnum"));
		}
		return Z_Registration_Info_UEnum_materialEnum.OuterSingleton;
	}
	template<> ASSETPLUGIN_API UEnum* StaticEnum<materialEnum>()
	{
		return materialEnum_StaticEnum();
	}
	struct Z_Construct_UEnum_AssetPlugin_materialEnum_Statics
	{
		static const UECodeGen_Private::FEnumeratorParam Enumerators[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[];
#endif
		static const UECodeGen_Private::FEnumParams EnumParams;
	};
	const UECodeGen_Private::FEnumeratorParam Z_Construct_UEnum_AssetPlugin_materialEnum_Statics::Enumerators[] = {
		{ "materialEnum::grassMaterial", (int64)materialEnum::grassMaterial },
		{ "materialEnum::wallMaterial", (int64)materialEnum::wallMaterial },
		{ "materialEnum::glassMaterial", (int64)materialEnum::glassMaterial },
		{ "materialEnum::stoneMaterial", (int64)materialEnum::stoneMaterial },
		{ "materialEnum::beigeStoneMaterial", (int64)materialEnum::beigeStoneMaterial },
		{ "materialEnum::sandMaterial", (int64)materialEnum::sandMaterial },
		{ "materialEnum::redsandMaterial", (int64)materialEnum::redsandMaterial },
		{ "materialEnum::treeMaterial", (int64)materialEnum::treeMaterial },
		{ "materialEnum::palmLeafMaterial", (int64)materialEnum::palmLeafMaterial },
		{ "materialEnum::waterMaterial", (int64)materialEnum::waterMaterial },
		{ "materialEnum::snowMaterial", (int64)materialEnum::snowMaterial },
		{ "materialEnum::prop_alarmBoxMaterial", (int64)materialEnum::prop_alarmBoxMaterial },
		{ "materialEnum::_texturedMaterial", (int64)materialEnum::_texturedMaterial },
		{ "materialEnum::wingMaterial", (int64)materialEnum::wingMaterial },
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UEnum_AssetPlugin_materialEnum_Statics::Enum_MetaDataParams[] = {
		{ "_texturedMaterial.Name", "materialEnum::_texturedMaterial" },
		{ "beigeStoneMaterial.Name", "materialEnum::beigeStoneMaterial" },
		{ "glassMaterial.Name", "materialEnum::glassMaterial" },
		{ "grassMaterial.Name", "materialEnum::grassMaterial" },
		{ "ModuleRelativePath", "gamestart/assetEnums/materialEnum.h" },
		{ "palmLeafMaterial.Name", "materialEnum::palmLeafMaterial" },
		{ "prop_alarmBoxMaterial.Name", "materialEnum::prop_alarmBoxMaterial" },
		{ "redsandMaterial.Name", "materialEnum::redsandMaterial" },
		{ "sandMaterial.Name", "materialEnum::sandMaterial" },
		{ "snowMaterial.Name", "materialEnum::snowMaterial" },
		{ "stoneMaterial.Name", "materialEnum::stoneMaterial" },
		{ "treeMaterial.Name", "materialEnum::treeMaterial" },
		{ "wallMaterial.Name", "materialEnum::wallMaterial" },
		{ "waterMaterial.Name", "materialEnum::waterMaterial" },
		{ "wingMaterial.Name", "materialEnum::wingMaterial" },
	};
#endif
	const UECodeGen_Private::FEnumParams Z_Construct_UEnum_AssetPlugin_materialEnum_Statics::EnumParams = {
		(UObject*(*)())Z_Construct_UPackage__Script_AssetPlugin,
		nullptr,
		"materialEnum",
		"materialEnum",
		Z_Construct_UEnum_AssetPlugin_materialEnum_Statics::Enumerators,
		UE_ARRAY_COUNT(Z_Construct_UEnum_AssetPlugin_materialEnum_Statics::Enumerators),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EEnumFlags::None,
		(uint8)UEnum::ECppForm::EnumClass,
		METADATA_PARAMS(Z_Construct_UEnum_AssetPlugin_materialEnum_Statics::Enum_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UEnum_AssetPlugin_materialEnum_Statics::Enum_MetaDataParams))
	};
	UEnum* Z_Construct_UEnum_AssetPlugin_materialEnum()
	{
		if (!Z_Registration_Info_UEnum_materialEnum.InnerSingleton)
		{
			UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_materialEnum.InnerSingleton, Z_Construct_UEnum_AssetPlugin_materialEnum_Statics::EnumParams);
		}
		return Z_Registration_Info_UEnum_materialEnum.InnerSingleton;
	}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_assetEnums_materialEnum_h_Statics
	{
		static const FEnumRegisterCompiledInInfo EnumInfo[];
	};
	const FEnumRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_assetEnums_materialEnum_h_Statics::EnumInfo[] = {
		{ materialEnum_StaticEnum, TEXT("materialEnum"), &Z_Registration_Info_UEnum_materialEnum, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 890177524U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_assetEnums_materialEnum_h_3701099565(TEXT("/Script/AssetPlugin"),
		nullptr, 0,
		nullptr, 0,
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_assetEnums_materialEnum_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_assetEnums_materialEnum_h_Statics::EnumInfo));
PRAGMA_ENABLE_DEPRECATION_WARNINGS

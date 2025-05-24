// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "AssetPlugin/gamestart/assetEnums/weaponAttachmentEnum.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeweaponAttachmentEnum() {}
// Cross Module References
	ASSETPLUGIN_API UEnum* Z_Construct_UEnum_AssetPlugin_weaponAttachmentEnum();
	UPackage* Z_Construct_UPackage__Script_AssetPlugin();
// End Cross Module References
	static FEnumRegistrationInfo Z_Registration_Info_UEnum_weaponAttachmentEnum;
	static UEnum* weaponAttachmentEnum_StaticEnum()
	{
		if (!Z_Registration_Info_UEnum_weaponAttachmentEnum.OuterSingleton)
		{
			Z_Registration_Info_UEnum_weaponAttachmentEnum.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_AssetPlugin_weaponAttachmentEnum, (UObject*)Z_Construct_UPackage__Script_AssetPlugin(), TEXT("weaponAttachmentEnum"));
		}
		return Z_Registration_Info_UEnum_weaponAttachmentEnum.OuterSingleton;
	}
	template<> ASSETPLUGIN_API UEnum* StaticEnum<weaponAttachmentEnum>()
	{
		return weaponAttachmentEnum_StaticEnum();
	}
	struct Z_Construct_UEnum_AssetPlugin_weaponAttachmentEnum_Statics
	{
		static const UECodeGen_Private::FEnumeratorParam Enumerators[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[];
#endif
		static const UECodeGen_Private::FEnumParams EnumParams;
	};
	const UECodeGen_Private::FEnumeratorParam Z_Construct_UEnum_AssetPlugin_weaponAttachmentEnum_Statics::Enumerators[] = {
		{ "weaponAttachmentEnum::iron_sight", (int64)weaponAttachmentEnum::iron_sight },
		{ "weaponAttachmentEnum::reddot", (int64)weaponAttachmentEnum::reddot },
		{ "weaponAttachmentEnum::grip_vertical", (int64)weaponAttachmentEnum::grip_vertical },
		{ "weaponAttachmentEnum::muzzle_flashSurpressor", (int64)weaponAttachmentEnum::muzzle_flashSurpressor },
		{ "weaponAttachmentEnum::muzzle_SoundSurpressor", (int64)weaponAttachmentEnum::muzzle_SoundSurpressor },
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UEnum_AssetPlugin_weaponAttachmentEnum_Statics::Enum_MetaDataParams[] = {
		{ "grip_vertical.Name", "weaponAttachmentEnum::grip_vertical" },
		{ "iron_sight.Name", "weaponAttachmentEnum::iron_sight" },
		{ "ModuleRelativePath", "gamestart/assetEnums/weaponAttachmentEnum.h" },
		{ "muzzle_flashSurpressor.Name", "weaponAttachmentEnum::muzzle_flashSurpressor" },
		{ "muzzle_SoundSurpressor.Name", "weaponAttachmentEnum::muzzle_SoundSurpressor" },
		{ "reddot.Name", "weaponAttachmentEnum::reddot" },
	};
#endif
	const UECodeGen_Private::FEnumParams Z_Construct_UEnum_AssetPlugin_weaponAttachmentEnum_Statics::EnumParams = {
		(UObject*(*)())Z_Construct_UPackage__Script_AssetPlugin,
		nullptr,
		"weaponAttachmentEnum",
		"weaponAttachmentEnum",
		Z_Construct_UEnum_AssetPlugin_weaponAttachmentEnum_Statics::Enumerators,
		UE_ARRAY_COUNT(Z_Construct_UEnum_AssetPlugin_weaponAttachmentEnum_Statics::Enumerators),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EEnumFlags::None,
		(uint8)UEnum::ECppForm::EnumClass,
		METADATA_PARAMS(Z_Construct_UEnum_AssetPlugin_weaponAttachmentEnum_Statics::Enum_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UEnum_AssetPlugin_weaponAttachmentEnum_Statics::Enum_MetaDataParams))
	};
	UEnum* Z_Construct_UEnum_AssetPlugin_weaponAttachmentEnum()
	{
		if (!Z_Registration_Info_UEnum_weaponAttachmentEnum.InnerSingleton)
		{
			UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_weaponAttachmentEnum.InnerSingleton, Z_Construct_UEnum_AssetPlugin_weaponAttachmentEnum_Statics::EnumParams);
		}
		return Z_Registration_Info_UEnum_weaponAttachmentEnum.InnerSingleton;
	}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_assetEnums_weaponAttachmentEnum_h_Statics
	{
		static const FEnumRegisterCompiledInInfo EnumInfo[];
	};
	const FEnumRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_assetEnums_weaponAttachmentEnum_h_Statics::EnumInfo[] = {
		{ weaponAttachmentEnum_StaticEnum, TEXT("weaponAttachmentEnum"), &Z_Registration_Info_UEnum_weaponAttachmentEnum, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 1842347311U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_assetEnums_weaponAttachmentEnum_h_1875047407(TEXT("/Script/AssetPlugin"),
		nullptr, 0,
		nullptr, 0,
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_assetEnums_weaponAttachmentEnum_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_assetEnums_weaponAttachmentEnum_h_Statics::EnumInfo));
PRAGMA_ENABLE_DEPRECATION_WARNINGS

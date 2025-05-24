// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "AssetPlugin/gamestart/assetEnums/weaponEnum.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeweaponEnum() {}
// Cross Module References
	ASSETPLUGIN_API UEnum* Z_Construct_UEnum_AssetPlugin_weaponEnum();
	UPackage* Z_Construct_UPackage__Script_AssetPlugin();
// End Cross Module References
	static FEnumRegistrationInfo Z_Registration_Info_UEnum_weaponEnum;
	static UEnum* weaponEnum_StaticEnum()
	{
		if (!Z_Registration_Info_UEnum_weaponEnum.OuterSingleton)
		{
			Z_Registration_Info_UEnum_weaponEnum.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_AssetPlugin_weaponEnum, (UObject*)Z_Construct_UPackage__Script_AssetPlugin(), TEXT("weaponEnum"));
		}
		return Z_Registration_Info_UEnum_weaponEnum.OuterSingleton;
	}
	template<> ASSETPLUGIN_API UEnum* StaticEnum<weaponEnum>()
	{
		return weaponEnum_StaticEnum();
	}
	struct Z_Construct_UEnum_AssetPlugin_weaponEnum_Statics
	{
		static const UECodeGen_Private::FEnumeratorParam Enumerators[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[];
#endif
		static const UECodeGen_Private::FEnumParams EnumParams;
	};
	const UECodeGen_Private::FEnumeratorParam Z_Construct_UEnum_AssetPlugin_weaponEnum_Statics::Enumerators[] = {
		{ "weaponEnum::assaultRifle", (int64)weaponEnum::assaultRifle },
		{ "weaponEnum::pistol", (int64)weaponEnum::pistol },
		{ "weaponEnum::thrower", (int64)weaponEnum::thrower },
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UEnum_AssetPlugin_weaponEnum_Statics::Enum_MetaDataParams[] = {
		{ "assaultRifle.Comment", "/**\n * \n */" },
		{ "assaultRifle.Name", "weaponEnum::assaultRifle" },
		{ "Comment", "/**\n * \n */" },
		{ "ModuleRelativePath", "gamestart/assetEnums/weaponEnum.h" },
		{ "pistol.Comment", "/**\n * \n */" },
		{ "pistol.Name", "weaponEnum::pistol" },
		{ "thrower.Comment", "/**\n * \n */" },
		{ "thrower.Name", "weaponEnum::thrower" },
	};
#endif
	const UECodeGen_Private::FEnumParams Z_Construct_UEnum_AssetPlugin_weaponEnum_Statics::EnumParams = {
		(UObject*(*)())Z_Construct_UPackage__Script_AssetPlugin,
		nullptr,
		"weaponEnum",
		"weaponEnum",
		Z_Construct_UEnum_AssetPlugin_weaponEnum_Statics::Enumerators,
		UE_ARRAY_COUNT(Z_Construct_UEnum_AssetPlugin_weaponEnum_Statics::Enumerators),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EEnumFlags::None,
		(uint8)UEnum::ECppForm::EnumClass,
		METADATA_PARAMS(Z_Construct_UEnum_AssetPlugin_weaponEnum_Statics::Enum_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UEnum_AssetPlugin_weaponEnum_Statics::Enum_MetaDataParams))
	};
	UEnum* Z_Construct_UEnum_AssetPlugin_weaponEnum()
	{
		if (!Z_Registration_Info_UEnum_weaponEnum.InnerSingleton)
		{
			UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_weaponEnum.InnerSingleton, Z_Construct_UEnum_AssetPlugin_weaponEnum_Statics::EnumParams);
		}
		return Z_Registration_Info_UEnum_weaponEnum.InnerSingleton;
	}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_assetEnums_weaponEnum_h_Statics
	{
		static const FEnumRegisterCompiledInInfo EnumInfo[];
	};
	const FEnumRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_assetEnums_weaponEnum_h_Statics::EnumInfo[] = {
		{ weaponEnum_StaticEnum, TEXT("weaponEnum"), &Z_Registration_Info_UEnum_weaponEnum, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 1150766490U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_assetEnums_weaponEnum_h_323673573(TEXT("/Script/AssetPlugin"),
		nullptr, 0,
		nullptr, 0,
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_assetEnums_weaponEnum_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_assetEnums_weaponEnum_h_Statics::EnumInfo));
PRAGMA_ENABLE_DEPRECATION_WARNINGS

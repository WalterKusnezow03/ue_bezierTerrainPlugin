// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "AssetPlugin/gamestart/assetEnums/textureEnum.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodetextureEnum() {}
// Cross Module References
	ASSETPLUGIN_API UEnum* Z_Construct_UEnum_AssetPlugin_textureEnum();
	UPackage* Z_Construct_UPackage__Script_AssetPlugin();
// End Cross Module References
	static FEnumRegistrationInfo Z_Registration_Info_UEnum_textureEnum;
	static UEnum* textureEnum_StaticEnum()
	{
		if (!Z_Registration_Info_UEnum_textureEnum.OuterSingleton)
		{
			Z_Registration_Info_UEnum_textureEnum.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_AssetPlugin_textureEnum, (UObject*)Z_Construct_UPackage__Script_AssetPlugin(), TEXT("textureEnum"));
		}
		return Z_Registration_Info_UEnum_textureEnum.OuterSingleton;
	}
	template<> ASSETPLUGIN_API UEnum* StaticEnum<textureEnum>()
	{
		return textureEnum_StaticEnum();
	}
	struct Z_Construct_UEnum_AssetPlugin_textureEnum_Statics
	{
		static const UECodeGen_Private::FEnumeratorParam Enumerators[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[];
#endif
		static const UECodeGen_Private::FEnumParams EnumParams;
	};
	const UECodeGen_Private::FEnumeratorParam Z_Construct_UEnum_AssetPlugin_textureEnum_Statics::Enumerators[] = {
		{ "textureEnum::patroneIcon", (int64)textureEnum::patroneIcon },
		{ "textureEnum::healthIcon", (int64)textureEnum::healthIcon },
		{ "textureEnum::enemyMarkerIcon", (int64)textureEnum::enemyMarkerIcon },
		{ "textureEnum::playerMarkerIcon", (int64)textureEnum::playerMarkerIcon },
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UEnum_AssetPlugin_textureEnum_Statics::Enum_MetaDataParams[] = {
		{ "enemyMarkerIcon.Name", "textureEnum::enemyMarkerIcon" },
		{ "healthIcon.Name", "textureEnum::healthIcon" },
		{ "ModuleRelativePath", "gamestart/assetEnums/textureEnum.h" },
		{ "patroneIcon.Name", "textureEnum::patroneIcon" },
		{ "playerMarkerIcon.Name", "textureEnum::playerMarkerIcon" },
	};
#endif
	const UECodeGen_Private::FEnumParams Z_Construct_UEnum_AssetPlugin_textureEnum_Statics::EnumParams = {
		(UObject*(*)())Z_Construct_UPackage__Script_AssetPlugin,
		nullptr,
		"textureEnum",
		"textureEnum",
		Z_Construct_UEnum_AssetPlugin_textureEnum_Statics::Enumerators,
		UE_ARRAY_COUNT(Z_Construct_UEnum_AssetPlugin_textureEnum_Statics::Enumerators),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EEnumFlags::None,
		(uint8)UEnum::ECppForm::EnumClass,
		METADATA_PARAMS(Z_Construct_UEnum_AssetPlugin_textureEnum_Statics::Enum_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UEnum_AssetPlugin_textureEnum_Statics::Enum_MetaDataParams))
	};
	UEnum* Z_Construct_UEnum_AssetPlugin_textureEnum()
	{
		if (!Z_Registration_Info_UEnum_textureEnum.InnerSingleton)
		{
			UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_textureEnum.InnerSingleton, Z_Construct_UEnum_AssetPlugin_textureEnum_Statics::EnumParams);
		}
		return Z_Registration_Info_UEnum_textureEnum.InnerSingleton;
	}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_assetEnums_textureEnum_h_Statics
	{
		static const FEnumRegisterCompiledInInfo EnumInfo[];
	};
	const FEnumRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_assetEnums_textureEnum_h_Statics::EnumInfo[] = {
		{ textureEnum_StaticEnum, TEXT("textureEnum"), &Z_Registration_Info_UEnum_textureEnum, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 2485983801U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_assetEnums_textureEnum_h_527963529(TEXT("/Script/AssetPlugin"),
		nullptr, 0,
		nullptr, 0,
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_assetEnums_textureEnum_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_assetEnums_textureEnum_h_Statics::EnumInfo));
PRAGMA_ENABLE_DEPRECATION_WARNINGS

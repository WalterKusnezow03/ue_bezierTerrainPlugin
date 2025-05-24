// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "AssetPlugin/gamestart/assetEnums/entityEnum.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeentityEnum() {}
// Cross Module References
	ASSETPLUGIN_API UEnum* Z_Construct_UEnum_AssetPlugin_entityEnum();
	UPackage* Z_Construct_UPackage__Script_AssetPlugin();
// End Cross Module References
	static FEnumRegistrationInfo Z_Registration_Info_UEnum_entityEnum;
	static UEnum* entityEnum_StaticEnum()
	{
		if (!Z_Registration_Info_UEnum_entityEnum.OuterSingleton)
		{
			Z_Registration_Info_UEnum_entityEnum.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_AssetPlugin_entityEnum, (UObject*)Z_Construct_UPackage__Script_AssetPlugin(), TEXT("entityEnum"));
		}
		return Z_Registration_Info_UEnum_entityEnum.OuterSingleton;
	}
	template<> ASSETPLUGIN_API UEnum* StaticEnum<entityEnum>()
	{
		return entityEnum_StaticEnum();
	}
	struct Z_Construct_UEnum_AssetPlugin_entityEnum_Statics
	{
		static const UECodeGen_Private::FEnumeratorParam Enumerators[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[];
#endif
		static const UECodeGen_Private::FEnumParams EnumParams;
	};
	const UECodeGen_Private::FEnumeratorParam Z_Construct_UEnum_AssetPlugin_entityEnum_Statics::Enumerators[] = {
		{ "entityEnum::human_enum", (int64)entityEnum::human_enum },
		{ "entityEnum::entity_enum", (int64)entityEnum::entity_enum },
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UEnum_AssetPlugin_entityEnum_Statics::Enum_MetaDataParams[] = {
		{ "entity_enum.Name", "entityEnum::entity_enum" },
		{ "human_enum.Name", "entityEnum::human_enum" },
		{ "ModuleRelativePath", "gamestart/assetEnums/entityEnum.h" },
	};
#endif
	const UECodeGen_Private::FEnumParams Z_Construct_UEnum_AssetPlugin_entityEnum_Statics::EnumParams = {
		(UObject*(*)())Z_Construct_UPackage__Script_AssetPlugin,
		nullptr,
		"entityEnum",
		"entityEnum",
		Z_Construct_UEnum_AssetPlugin_entityEnum_Statics::Enumerators,
		UE_ARRAY_COUNT(Z_Construct_UEnum_AssetPlugin_entityEnum_Statics::Enumerators),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EEnumFlags::None,
		(uint8)UEnum::ECppForm::EnumClass,
		METADATA_PARAMS(Z_Construct_UEnum_AssetPlugin_entityEnum_Statics::Enum_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UEnum_AssetPlugin_entityEnum_Statics::Enum_MetaDataParams))
	};
	UEnum* Z_Construct_UEnum_AssetPlugin_entityEnum()
	{
		if (!Z_Registration_Info_UEnum_entityEnum.InnerSingleton)
		{
			UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_entityEnum.InnerSingleton, Z_Construct_UEnum_AssetPlugin_entityEnum_Statics::EnumParams);
		}
		return Z_Registration_Info_UEnum_entityEnum.InnerSingleton;
	}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_assetEnums_entityEnum_h_Statics
	{
		static const FEnumRegisterCompiledInInfo EnumInfo[];
	};
	const FEnumRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_assetEnums_entityEnum_h_Statics::EnumInfo[] = {
		{ entityEnum_StaticEnum, TEXT("entityEnum"), &Z_Registration_Info_UEnum_entityEnum, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 335485768U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_assetEnums_entityEnum_h_1667738466(TEXT("/Script/AssetPlugin"),
		nullptr, 0,
		nullptr, 0,
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_assetEnums_entityEnum_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_assetEnums_entityEnum_h_Statics::EnumInfo));
PRAGMA_ENABLE_DEPRECATION_WARNINGS

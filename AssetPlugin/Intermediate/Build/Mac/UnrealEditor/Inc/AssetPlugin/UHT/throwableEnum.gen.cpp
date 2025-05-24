// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "AssetPlugin/gamestart/assetEnums/throwableEnum.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodethrowableEnum() {}
// Cross Module References
	ASSETPLUGIN_API UEnum* Z_Construct_UEnum_AssetPlugin_throwableEnum();
	UPackage* Z_Construct_UPackage__Script_AssetPlugin();
// End Cross Module References
	static FEnumRegistrationInfo Z_Registration_Info_UEnum_throwableEnum;
	static UEnum* throwableEnum_StaticEnum()
	{
		if (!Z_Registration_Info_UEnum_throwableEnum.OuterSingleton)
		{
			Z_Registration_Info_UEnum_throwableEnum.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_AssetPlugin_throwableEnum, (UObject*)Z_Construct_UPackage__Script_AssetPlugin(), TEXT("throwableEnum"));
		}
		return Z_Registration_Info_UEnum_throwableEnum.OuterSingleton;
	}
	template<> ASSETPLUGIN_API UEnum* StaticEnum<throwableEnum>()
	{
		return throwableEnum_StaticEnum();
	}
	struct Z_Construct_UEnum_AssetPlugin_throwableEnum_Statics
	{
		static const UECodeGen_Private::FEnumeratorParam Enumerators[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[];
#endif
		static const UECodeGen_Private::FEnumParams EnumParams;
	};
	const UECodeGen_Private::FEnumeratorParam Z_Construct_UEnum_AssetPlugin_throwableEnum_Statics::Enumerators[] = {
		{ "throwableEnum::rock_enum", (int64)throwableEnum::rock_enum },
		{ "throwableEnum::greneade_enum", (int64)throwableEnum::greneade_enum },
		{ "throwableEnum::rocket_enum", (int64)throwableEnum::rocket_enum },
		{ "throwableEnum::molotov_enum", (int64)throwableEnum::molotov_enum },
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UEnum_AssetPlugin_throwableEnum_Statics::Enum_MetaDataParams[] = {
		{ "greneade_enum.Name", "throwableEnum::greneade_enum" },
		{ "ModuleRelativePath", "gamestart/assetEnums/throwableEnum.h" },
		{ "molotov_enum.Name", "throwableEnum::molotov_enum" },
		{ "rock_enum.Name", "throwableEnum::rock_enum" },
		{ "rocket_enum.Name", "throwableEnum::rocket_enum" },
	};
#endif
	const UECodeGen_Private::FEnumParams Z_Construct_UEnum_AssetPlugin_throwableEnum_Statics::EnumParams = {
		(UObject*(*)())Z_Construct_UPackage__Script_AssetPlugin,
		nullptr,
		"throwableEnum",
		"throwableEnum",
		Z_Construct_UEnum_AssetPlugin_throwableEnum_Statics::Enumerators,
		UE_ARRAY_COUNT(Z_Construct_UEnum_AssetPlugin_throwableEnum_Statics::Enumerators),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EEnumFlags::None,
		(uint8)UEnum::ECppForm::EnumClass,
		METADATA_PARAMS(Z_Construct_UEnum_AssetPlugin_throwableEnum_Statics::Enum_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UEnum_AssetPlugin_throwableEnum_Statics::Enum_MetaDataParams))
	};
	UEnum* Z_Construct_UEnum_AssetPlugin_throwableEnum()
	{
		if (!Z_Registration_Info_UEnum_throwableEnum.InnerSingleton)
		{
			UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_throwableEnum.InnerSingleton, Z_Construct_UEnum_AssetPlugin_throwableEnum_Statics::EnumParams);
		}
		return Z_Registration_Info_UEnum_throwableEnum.InnerSingleton;
	}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_assetEnums_throwableEnum_h_Statics
	{
		static const FEnumRegisterCompiledInInfo EnumInfo[];
	};
	const FEnumRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_assetEnums_throwableEnum_h_Statics::EnumInfo[] = {
		{ throwableEnum_StaticEnum, TEXT("throwableEnum"), &Z_Registration_Info_UEnum_throwableEnum, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 665914805U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_assetEnums_throwableEnum_h_2226225226(TEXT("/Script/AssetPlugin"),
		nullptr, 0,
		nullptr, 0,
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_assetEnums_throwableEnum_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_assetEnums_throwableEnum_h_Statics::EnumInfo));
PRAGMA_ENABLE_DEPRECATION_WARNINGS

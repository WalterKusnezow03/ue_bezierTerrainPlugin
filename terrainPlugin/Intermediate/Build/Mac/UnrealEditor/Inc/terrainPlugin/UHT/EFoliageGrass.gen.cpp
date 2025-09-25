// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "terrainPlugin/AssetEnums/EFoliageGrass.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeEFoliageGrass() {}
// Cross Module References
	TERRAINPLUGIN_API UEnum* Z_Construct_UEnum_terrainPlugin_EFoliageGrass();
	UPackage* Z_Construct_UPackage__Script_terrainPlugin();
// End Cross Module References
	static FEnumRegistrationInfo Z_Registration_Info_UEnum_EFoliageGrass;
	static UEnum* EFoliageGrass_StaticEnum()
	{
		if (!Z_Registration_Info_UEnum_EFoliageGrass.OuterSingleton)
		{
			Z_Registration_Info_UEnum_EFoliageGrass.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_terrainPlugin_EFoliageGrass, (UObject*)Z_Construct_UPackage__Script_terrainPlugin(), TEXT("EFoliageGrass"));
		}
		return Z_Registration_Info_UEnum_EFoliageGrass.OuterSingleton;
	}
	template<> TERRAINPLUGIN_API UEnum* StaticEnum<EFoliageGrass>()
	{
		return EFoliageGrass_StaticEnum();
	}
	struct Z_Construct_UEnum_terrainPlugin_EFoliageGrass_Statics
	{
		static const UECodeGen_Private::FEnumeratorParam Enumerators[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[];
#endif
		static const UECodeGen_Private::FEnumParams EnumParams;
	};
	const UECodeGen_Private::FEnumeratorParam Z_Construct_UEnum_terrainPlugin_EFoliageGrass_Statics::Enumerators[] = {
		{ "EFoliageGrass::grassAssetDefault", (int64)EFoliageGrass::grassAssetDefault },
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UEnum_terrainPlugin_EFoliageGrass_Statics::Enum_MetaDataParams[] = {
		{ "grassAssetDefault.Name", "EFoliageGrass::grassAssetDefault" },
		{ "ModuleRelativePath", "AssetEnums/EFoliageGrass.h" },
	};
#endif
	const UECodeGen_Private::FEnumParams Z_Construct_UEnum_terrainPlugin_EFoliageGrass_Statics::EnumParams = {
		(UObject*(*)())Z_Construct_UPackage__Script_terrainPlugin,
		nullptr,
		"EFoliageGrass",
		"EFoliageGrass",
		Z_Construct_UEnum_terrainPlugin_EFoliageGrass_Statics::Enumerators,
		UE_ARRAY_COUNT(Z_Construct_UEnum_terrainPlugin_EFoliageGrass_Statics::Enumerators),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EEnumFlags::None,
		(uint8)UEnum::ECppForm::EnumClass,
		METADATA_PARAMS(Z_Construct_UEnum_terrainPlugin_EFoliageGrass_Statics::Enum_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UEnum_terrainPlugin_EFoliageGrass_Statics::Enum_MetaDataParams))
	};
	UEnum* Z_Construct_UEnum_terrainPlugin_EFoliageGrass()
	{
		if (!Z_Registration_Info_UEnum_EFoliageGrass.InnerSingleton)
		{
			UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EFoliageGrass.InnerSingleton, Z_Construct_UEnum_terrainPlugin_EFoliageGrass_Statics::EnumParams);
		}
		return Z_Registration_Info_UEnum_EFoliageGrass.InnerSingleton;
	}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_AssetEnums_EFoliageGrass_h_Statics
	{
		static const FEnumRegisterCompiledInInfo EnumInfo[];
	};
	const FEnumRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_AssetEnums_EFoliageGrass_h_Statics::EnumInfo[] = {
		{ EFoliageGrass_StaticEnum, TEXT("EFoliageGrass"), &Z_Registration_Info_UEnum_EFoliageGrass, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 976001739U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_AssetEnums_EFoliageGrass_h_3359388718(TEXT("/Script/terrainPlugin"),
		nullptr, 0,
		nullptr, 0,
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_AssetEnums_EFoliageGrass_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_AssetEnums_EFoliageGrass_h_Statics::EnumInfo));
PRAGMA_ENABLE_DEPRECATION_WARNINGS

// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "terrainPlugin/meshgen/foliage/MatrixTree/ETreeType.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeETreeType() {}
// Cross Module References
	TERRAINPLUGIN_API UEnum* Z_Construct_UEnum_terrainPlugin_ETreeType();
	UPackage* Z_Construct_UPackage__Script_terrainPlugin();
// End Cross Module References
	static FEnumRegistrationInfo Z_Registration_Info_UEnum_ETreeType;
	static UEnum* ETreeType_StaticEnum()
	{
		if (!Z_Registration_Info_UEnum_ETreeType.OuterSingleton)
		{
			Z_Registration_Info_UEnum_ETreeType.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_terrainPlugin_ETreeType, (UObject*)Z_Construct_UPackage__Script_terrainPlugin(), TEXT("ETreeType"));
		}
		return Z_Registration_Info_UEnum_ETreeType.OuterSingleton;
	}
	template<> TERRAINPLUGIN_API UEnum* StaticEnum<ETreeType>()
	{
		return ETreeType_StaticEnum();
	}
	struct Z_Construct_UEnum_terrainPlugin_ETreeType_Statics
	{
		static const UECodeGen_Private::FEnumeratorParam Enumerators[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[];
#endif
		static const UECodeGen_Private::FEnumParams EnumParams;
	};
	const UECodeGen_Private::FEnumeratorParam Z_Construct_UEnum_terrainPlugin_ETreeType_Statics::Enumerators[] = {
		{ "ETreeType::Edefault", (int64)ETreeType::Edefault },
		{ "ETreeType::EPalmTree", (int64)ETreeType::EPalmTree },
		{ "ETreeType::EPalmBush", (int64)ETreeType::EPalmBush },
		{ "ETreeType::ECactus", (int64)ETreeType::ECactus },
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UEnum_terrainPlugin_ETreeType_Statics::Enum_MetaDataParams[] = {
		{ "ECactus.Name", "ETreeType::ECactus" },
		{ "Edefault.Name", "ETreeType::Edefault" },
		{ "EPalmBush.Name", "ETreeType::EPalmBush" },
		{ "EPalmTree.Name", "ETreeType::EPalmTree" },
		{ "ModuleRelativePath", "meshgen/foliage/MatrixTree/ETreeType.h" },
	};
#endif
	const UECodeGen_Private::FEnumParams Z_Construct_UEnum_terrainPlugin_ETreeType_Statics::EnumParams = {
		(UObject*(*)())Z_Construct_UPackage__Script_terrainPlugin,
		nullptr,
		"ETreeType",
		"ETreeType",
		Z_Construct_UEnum_terrainPlugin_ETreeType_Statics::Enumerators,
		UE_ARRAY_COUNT(Z_Construct_UEnum_terrainPlugin_ETreeType_Statics::Enumerators),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EEnumFlags::None,
		(uint8)UEnum::ECppForm::EnumClass,
		METADATA_PARAMS(Z_Construct_UEnum_terrainPlugin_ETreeType_Statics::Enum_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UEnum_terrainPlugin_ETreeType_Statics::Enum_MetaDataParams))
	};
	UEnum* Z_Construct_UEnum_terrainPlugin_ETreeType()
	{
		if (!Z_Registration_Info_UEnum_ETreeType.InnerSingleton)
		{
			UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_ETreeType.InnerSingleton, Z_Construct_UEnum_terrainPlugin_ETreeType_Statics::EnumParams);
		}
		return Z_Registration_Info_UEnum_ETreeType.InnerSingleton;
	}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_foliage_MatrixTree_ETreeType_h_Statics
	{
		static const FEnumRegisterCompiledInInfo EnumInfo[];
	};
	const FEnumRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_foliage_MatrixTree_ETreeType_h_Statics::EnumInfo[] = {
		{ ETreeType_StaticEnum, TEXT("ETreeType"), &Z_Registration_Info_UEnum_ETreeType, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 802346641U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_foliage_MatrixTree_ETreeType_h_3085803680(TEXT("/Script/terrainPlugin"),
		nullptr, 0,
		nullptr, 0,
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_foliage_MatrixTree_ETreeType_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_foliage_MatrixTree_ETreeType_h_Statics::EnumInfo));
PRAGMA_ENABLE_DEPRECATION_WARNINGS

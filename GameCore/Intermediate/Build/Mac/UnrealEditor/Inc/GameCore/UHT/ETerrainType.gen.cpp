// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "GameCore/MeshGenBase/foliage/ETerrainType.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeETerrainType() {}
// Cross Module References
	GAMECORE_API UEnum* Z_Construct_UEnum_GameCore_ETerrainType();
	UPackage* Z_Construct_UPackage__Script_GameCore();
// End Cross Module References
	static FEnumRegistrationInfo Z_Registration_Info_UEnum_ETerrainType;
	static UEnum* ETerrainType_StaticEnum()
	{
		if (!Z_Registration_Info_UEnum_ETerrainType.OuterSingleton)
		{
			Z_Registration_Info_UEnum_ETerrainType.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_GameCore_ETerrainType, (UObject*)Z_Construct_UPackage__Script_GameCore(), TEXT("ETerrainType"));
		}
		return Z_Registration_Info_UEnum_ETerrainType.OuterSingleton;
	}
	template<> GAMECORE_API UEnum* StaticEnum<ETerrainType>()
	{
		return ETerrainType_StaticEnum();
	}
	struct Z_Construct_UEnum_GameCore_ETerrainType_Statics
	{
		static const UECodeGen_Private::FEnumeratorParam Enumerators[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[];
#endif
		static const UECodeGen_Private::FEnumParams EnumParams;
	};
	const UECodeGen_Private::FEnumeratorParam Z_Construct_UEnum_GameCore_ETerrainType_Statics::Enumerators[] = {
		{ "ETerrainType::EForest", (int64)ETerrainType::EForest },
		{ "ETerrainType::ETropical", (int64)ETerrainType::ETropical },
		{ "ETerrainType::EDesert", (int64)ETerrainType::EDesert },
		{ "ETerrainType::EDesertForest", (int64)ETerrainType::EDesertForest },
		{ "ETerrainType::EOcean", (int64)ETerrainType::EOcean },
		{ "ETerrainType::ESnowHill", (int64)ETerrainType::ESnowHill },
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UEnum_GameCore_ETerrainType_Statics::Enum_MetaDataParams[] = {
		{ "EDesert.Name", "ETerrainType::EDesert" },
		{ "EDesertForest.Name", "ETerrainType::EDesertForest" },
		{ "EForest.Name", "ETerrainType::EForest" },
		{ "EOcean.Name", "ETerrainType::EOcean" },
		{ "ESnowHill.Name", "ETerrainType::ESnowHill" },
		{ "ETropical.Name", "ETerrainType::ETropical" },
		{ "ModuleRelativePath", "MeshGenBase/foliage/ETerrainType.h" },
	};
#endif
	const UECodeGen_Private::FEnumParams Z_Construct_UEnum_GameCore_ETerrainType_Statics::EnumParams = {
		(UObject*(*)())Z_Construct_UPackage__Script_GameCore,
		nullptr,
		"ETerrainType",
		"ETerrainType",
		Z_Construct_UEnum_GameCore_ETerrainType_Statics::Enumerators,
		UE_ARRAY_COUNT(Z_Construct_UEnum_GameCore_ETerrainType_Statics::Enumerators),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EEnumFlags::None,
		(uint8)UEnum::ECppForm::EnumClass,
		METADATA_PARAMS(Z_Construct_UEnum_GameCore_ETerrainType_Statics::Enum_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UEnum_GameCore_ETerrainType_Statics::Enum_MetaDataParams))
	};
	UEnum* Z_Construct_UEnum_GameCore_ETerrainType()
	{
		if (!Z_Registration_Info_UEnum_ETerrainType.InnerSingleton)
		{
			UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_ETerrainType.InnerSingleton, Z_Construct_UEnum_GameCore_ETerrainType_Statics::EnumParams);
		}
		return Z_Registration_Info_UEnum_ETerrainType.InnerSingleton;
	}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_GameCore_Source_GameCore_MeshGenBase_foliage_ETerrainType_h_Statics
	{
		static const FEnumRegisterCompiledInInfo EnumInfo[];
	};
	const FEnumRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_GameCore_Source_GameCore_MeshGenBase_foliage_ETerrainType_h_Statics::EnumInfo[] = {
		{ ETerrainType_StaticEnum, TEXT("ETerrainType"), &Z_Registration_Info_UEnum_ETerrainType, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 667884764U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_GameCore_Source_GameCore_MeshGenBase_foliage_ETerrainType_h_491249349(TEXT("/Script/GameCore"),
		nullptr, 0,
		nullptr, 0,
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_GameCore_Source_GameCore_MeshGenBase_foliage_ETerrainType_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_GameCore_Source_GameCore_MeshGenBase_foliage_ETerrainType_h_Statics::EnumInfo));
PRAGMA_ENABLE_DEPRECATION_WARNINGS

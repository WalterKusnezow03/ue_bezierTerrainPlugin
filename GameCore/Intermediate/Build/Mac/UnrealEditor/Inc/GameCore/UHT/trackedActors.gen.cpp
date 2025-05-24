// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "GameCore/EntityGC/trackedActors.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodetrackedActors() {}
// Cross Module References
	GAMECORE_API UEnum* Z_Construct_UEnum_GameCore_ETrackedActors();
	UPackage* Z_Construct_UPackage__Script_GameCore();
// End Cross Module References
	static FEnumRegistrationInfo Z_Registration_Info_UEnum_ETrackedActors;
	static UEnum* ETrackedActors_StaticEnum()
	{
		if (!Z_Registration_Info_UEnum_ETrackedActors.OuterSingleton)
		{
			Z_Registration_Info_UEnum_ETrackedActors.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_GameCore_ETrackedActors, (UObject*)Z_Construct_UPackage__Script_GameCore(), TEXT("ETrackedActors"));
		}
		return Z_Registration_Info_UEnum_ETrackedActors.OuterSingleton;
	}
	template<> GAMECORE_API UEnum* StaticEnum<ETrackedActors>()
	{
		return ETrackedActors_StaticEnum();
	}
	struct Z_Construct_UEnum_GameCore_ETrackedActors_Statics
	{
		static const UECodeGen_Private::FEnumeratorParam Enumerators[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[];
#endif
		static const UECodeGen_Private::FEnumParams EnumParams;
	};
	const UECodeGen_Private::FEnumeratorParam Z_Construct_UEnum_GameCore_ETrackedActors_Statics::Enumerators[] = {
		{ "ETrackedActors::EMeshActor", (int64)ETrackedActors::EMeshActor },
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UEnum_GameCore_ETrackedActors_Statics::Enum_MetaDataParams[] = {
		{ "Comment", "/** \n * Tracked actor classes which are unique and do\n * not have sub types by enum like entity team \n * or weapon types etc.\n * \n*/" },
		{ "EMeshActor.Name", "ETrackedActors::EMeshActor" },
		{ "ModuleRelativePath", "EntityGC/trackedActors.h" },
		{ "ToolTip", "Tracked actor classes which are unique and do\nnot have sub types by enum like entity team\nor weapon types etc." },
	};
#endif
	const UECodeGen_Private::FEnumParams Z_Construct_UEnum_GameCore_ETrackedActors_Statics::EnumParams = {
		(UObject*(*)())Z_Construct_UPackage__Script_GameCore,
		nullptr,
		"ETrackedActors",
		"ETrackedActors",
		Z_Construct_UEnum_GameCore_ETrackedActors_Statics::Enumerators,
		UE_ARRAY_COUNT(Z_Construct_UEnum_GameCore_ETrackedActors_Statics::Enumerators),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EEnumFlags::None,
		(uint8)UEnum::ECppForm::EnumClass,
		METADATA_PARAMS(Z_Construct_UEnum_GameCore_ETrackedActors_Statics::Enum_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UEnum_GameCore_ETrackedActors_Statics::Enum_MetaDataParams))
	};
	UEnum* Z_Construct_UEnum_GameCore_ETrackedActors()
	{
		if (!Z_Registration_Info_UEnum_ETrackedActors.InnerSingleton)
		{
			UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_ETrackedActors.InnerSingleton, Z_Construct_UEnum_GameCore_ETrackedActors_Statics::EnumParams);
		}
		return Z_Registration_Info_UEnum_ETrackedActors.InnerSingleton;
	}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_GameCore_Source_GameCore_EntityGC_trackedActors_h_Statics
	{
		static const FEnumRegisterCompiledInInfo EnumInfo[];
	};
	const FEnumRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_GameCore_Source_GameCore_EntityGC_trackedActors_h_Statics::EnumInfo[] = {
		{ ETrackedActors_StaticEnum, TEXT("ETrackedActors"), &Z_Registration_Info_UEnum_ETrackedActors, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 2976940613U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_GameCore_Source_GameCore_EntityGC_trackedActors_h_3189107149(TEXT("/Script/GameCore"),
		nullptr, 0,
		nullptr, 0,
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_GameCore_Source_GameCore_EntityGC_trackedActors_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_GameCore_Source_GameCore_EntityGC_trackedActors_h_Statics::EnumInfo));
PRAGMA_ENABLE_DEPRECATION_WARNINGS

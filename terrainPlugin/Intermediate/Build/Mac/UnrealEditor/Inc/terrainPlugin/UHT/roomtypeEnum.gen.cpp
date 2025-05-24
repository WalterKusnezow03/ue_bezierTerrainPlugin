// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "terrainPlugin/meshgen/rooms/roomtypeEnum.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCoderoomtypeEnum() {}
// Cross Module References
	TERRAINPLUGIN_API UEnum* Z_Construct_UEnum_terrainPlugin_roomtypeEnum();
	UPackage* Z_Construct_UPackage__Script_terrainPlugin();
// End Cross Module References
	static FEnumRegistrationInfo Z_Registration_Info_UEnum_roomtypeEnum;
	static UEnum* roomtypeEnum_StaticEnum()
	{
		if (!Z_Registration_Info_UEnum_roomtypeEnum.OuterSingleton)
		{
			Z_Registration_Info_UEnum_roomtypeEnum.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_terrainPlugin_roomtypeEnum, (UObject*)Z_Construct_UPackage__Script_terrainPlugin(), TEXT("roomtypeEnum"));
		}
		return Z_Registration_Info_UEnum_roomtypeEnum.OuterSingleton;
	}
	template<> TERRAINPLUGIN_API UEnum* StaticEnum<roomtypeEnum>()
	{
		return roomtypeEnum_StaticEnum();
	}
	struct Z_Construct_UEnum_terrainPlugin_roomtypeEnum_Statics
	{
		static const UECodeGen_Private::FEnumeratorParam Enumerators[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[];
#endif
		static const UECodeGen_Private::FEnumParams EnumParams;
	};
	const UECodeGen_Private::FEnumeratorParam Z_Construct_UEnum_terrainPlugin_roomtypeEnum_Statics::Enumerators[] = {
		{ "roomtypeEnum::room", (int64)roomtypeEnum::room },
		{ "roomtypeEnum::staircase", (int64)roomtypeEnum::staircase },
		{ "roomtypeEnum::staircaseTopper", (int64)roomtypeEnum::staircaseTopper },
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UEnum_terrainPlugin_roomtypeEnum_Statics::Enum_MetaDataParams[] = {
		{ "Comment", "/**\n * the room type enum is designed for each room.cpp file to\n * hold its type to be mapped in the room manager properly\n */" },
		{ "ModuleRelativePath", "meshgen/rooms/roomtypeEnum.h" },
		{ "room.Name", "roomtypeEnum::room" },
		{ "staircase.Name", "roomtypeEnum::staircase" },
		{ "staircaseTopper.Name", "roomtypeEnum::staircaseTopper" },
		{ "ToolTip", "the room type enum is designed for each room.cpp file to\nhold its type to be mapped in the room manager properly" },
	};
#endif
	const UECodeGen_Private::FEnumParams Z_Construct_UEnum_terrainPlugin_roomtypeEnum_Statics::EnumParams = {
		(UObject*(*)())Z_Construct_UPackage__Script_terrainPlugin,
		nullptr,
		"roomtypeEnum",
		"roomtypeEnum",
		Z_Construct_UEnum_terrainPlugin_roomtypeEnum_Statics::Enumerators,
		UE_ARRAY_COUNT(Z_Construct_UEnum_terrainPlugin_roomtypeEnum_Statics::Enumerators),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EEnumFlags::None,
		(uint8)UEnum::ECppForm::EnumClass,
		METADATA_PARAMS(Z_Construct_UEnum_terrainPlugin_roomtypeEnum_Statics::Enum_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UEnum_terrainPlugin_roomtypeEnum_Statics::Enum_MetaDataParams))
	};
	UEnum* Z_Construct_UEnum_terrainPlugin_roomtypeEnum()
	{
		if (!Z_Registration_Info_UEnum_roomtypeEnum.InnerSingleton)
		{
			UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_roomtypeEnum.InnerSingleton, Z_Construct_UEnum_terrainPlugin_roomtypeEnum_Statics::EnumParams);
		}
		return Z_Registration_Info_UEnum_roomtypeEnum.InnerSingleton;
	}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_rooms_roomtypeEnum_h_Statics
	{
		static const FEnumRegisterCompiledInInfo EnumInfo[];
	};
	const FEnumRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_rooms_roomtypeEnum_h_Statics::EnumInfo[] = {
		{ roomtypeEnum_StaticEnum, TEXT("roomtypeEnum"), &Z_Registration_Info_UEnum_roomtypeEnum, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 3967587282U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_rooms_roomtypeEnum_h_2517698456(TEXT("/Script/terrainPlugin"),
		nullptr, 0,
		nullptr, 0,
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_rooms_roomtypeEnum_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_meshgen_rooms_roomtypeEnum_h_Statics::EnumInfo));
PRAGMA_ENABLE_DEPRECATION_WARNINGS

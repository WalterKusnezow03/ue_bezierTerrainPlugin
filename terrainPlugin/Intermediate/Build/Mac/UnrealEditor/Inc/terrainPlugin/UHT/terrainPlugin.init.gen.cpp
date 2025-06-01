// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeterrainPlugin_init() {}
	static FPackageRegistrationInfo Z_Registration_Info_UPackage__Script_terrainPlugin;
	FORCENOINLINE UPackage* Z_Construct_UPackage__Script_terrainPlugin()
	{
		if (!Z_Registration_Info_UPackage__Script_terrainPlugin.OuterSingleton)
		{
			static const UECodeGen_Private::FPackageParams PackageParams = {
				"/Script/terrainPlugin",
				nullptr,
				0,
				PKG_CompiledIn | 0x00000000,
				0xBFD95D1D,
				0x03ACCECB,
				METADATA_PARAMS(nullptr, 0)
			};
			UECodeGen_Private::ConstructUPackage(Z_Registration_Info_UPackage__Script_terrainPlugin.OuterSingleton, PackageParams);
		}
		return Z_Registration_Info_UPackage__Script_terrainPlugin.OuterSingleton;
	}
	static FRegisterCompiledInInfo Z_CompiledInDeferPackage_UPackage__Script_terrainPlugin(Z_Construct_UPackage__Script_terrainPlugin, TEXT("/Script/terrainPlugin"), Z_Registration_Info_UPackage__Script_terrainPlugin, CONSTRUCT_RELOAD_VERSION_INFO(FPackageReloadVersionInfo, 0xBFD95D1D, 0x03ACCECB));
PRAGMA_ENABLE_DEPRECATION_WARNINGS

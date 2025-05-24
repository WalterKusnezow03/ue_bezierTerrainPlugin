// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeAssetPlugin_init() {}
	static FPackageRegistrationInfo Z_Registration_Info_UPackage__Script_AssetPlugin;
	FORCENOINLINE UPackage* Z_Construct_UPackage__Script_AssetPlugin()
	{
		if (!Z_Registration_Info_UPackage__Script_AssetPlugin.OuterSingleton)
		{
			static const UECodeGen_Private::FPackageParams PackageParams = {
				"/Script/AssetPlugin",
				nullptr,
				0,
				PKG_CompiledIn | 0x00000000,
				0x5DB627B0,
				0xE4852FEE,
				METADATA_PARAMS(nullptr, 0)
			};
			UECodeGen_Private::ConstructUPackage(Z_Registration_Info_UPackage__Script_AssetPlugin.OuterSingleton, PackageParams);
		}
		return Z_Registration_Info_UPackage__Script_AssetPlugin.OuterSingleton;
	}
	static FRegisterCompiledInInfo Z_CompiledInDeferPackage_UPackage__Script_AssetPlugin(Z_Construct_UPackage__Script_AssetPlugin, TEXT("/Script/AssetPlugin"), Z_Registration_Info_UPackage__Script_AssetPlugin, CONSTRUCT_RELOAD_VERSION_INFO(FPackageReloadVersionInfo, 0x5DB627B0, 0xE4852FEE));
PRAGMA_ENABLE_DEPRECATION_WARNINGS

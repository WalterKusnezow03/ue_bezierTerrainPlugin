// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "GameCore/PlayerControllerBase/PlayerControllerBase.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePlayerControllerBase() {}
// Cross Module References
	ENGINE_API UClass* Z_Construct_UClass_ACharacter();
	ENGINE_API UClass* Z_Construct_UClass_UCameraComponent_NoRegister();
	GAMECORE_API UClass* Z_Construct_UClass_APlayerControllerBase();
	GAMECORE_API UClass* Z_Construct_UClass_APlayerControllerBase_NoRegister();
	GAMECORE_API UClass* Z_Construct_UClass_UDamageinterface_NoRegister();
	GAMECORE_API UClass* Z_Construct_UClass_USteeringinterface_NoRegister();
	UPackage* Z_Construct_UPackage__Script_GameCore();
// End Cross Module References
	void APlayerControllerBase::StaticRegisterNativesAPlayerControllerBase()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(APlayerControllerBase);
	UClass* Z_Construct_UClass_APlayerControllerBase_NoRegister()
	{
		return APlayerControllerBase::StaticClass();
	}
	struct Z_Construct_UClass_APlayerControllerBase_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_CameraComponent_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_CameraComponent;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_CameraComponentSecondary_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_CameraComponentSecondary;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_TurnRateGamepad_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_TurnRateGamepad;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_LookUpRateGamepad_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_LookUpRateGamepad;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UECodeGen_Private::FImplementedInterfaceParams InterfaceParams[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_APlayerControllerBase_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_ACharacter,
		(UObject* (*)())Z_Construct_UPackage__Script_GameCore,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_APlayerControllerBase_Statics::Class_MetaDataParams[] = {
		{ "HideCategories", "Navigation" },
		{ "IncludePath", "PlayerControllerBase/PlayerControllerBase.h" },
		{ "ModuleRelativePath", "PlayerControllerBase/PlayerControllerBase.h" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_APlayerControllerBase_Statics::NewProp_CameraComponent_MetaData[] = {
		{ "Category", "PlayerControllerBase" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "PlayerControllerBase/PlayerControllerBase.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_APlayerControllerBase_Statics::NewProp_CameraComponent = { "CameraComponent", nullptr, (EPropertyFlags)0x00100000000a0009, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(APlayerControllerBase, CameraComponent), Z_Construct_UClass_UCameraComponent_NoRegister, METADATA_PARAMS(Z_Construct_UClass_APlayerControllerBase_Statics::NewProp_CameraComponent_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_APlayerControllerBase_Statics::NewProp_CameraComponent_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_APlayerControllerBase_Statics::NewProp_CameraComponentSecondary_MetaData[] = {
		{ "Category", "PlayerControllerBase" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "PlayerControllerBase/PlayerControllerBase.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_APlayerControllerBase_Statics::NewProp_CameraComponentSecondary = { "CameraComponentSecondary", nullptr, (EPropertyFlags)0x00100000000a0009, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(APlayerControllerBase, CameraComponentSecondary), Z_Construct_UClass_UCameraComponent_NoRegister, METADATA_PARAMS(Z_Construct_UClass_APlayerControllerBase_Statics::NewProp_CameraComponentSecondary_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_APlayerControllerBase_Statics::NewProp_CameraComponentSecondary_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_APlayerControllerBase_Statics::NewProp_TurnRateGamepad_MetaData[] = {
		{ "Category", "PlayerControllerBase" },
		{ "ModuleRelativePath", "PlayerControllerBase/PlayerControllerBase.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_APlayerControllerBase_Statics::NewProp_TurnRateGamepad = { "TurnRateGamepad", nullptr, (EPropertyFlags)0x0020080000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(APlayerControllerBase, TurnRateGamepad), METADATA_PARAMS(Z_Construct_UClass_APlayerControllerBase_Statics::NewProp_TurnRateGamepad_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_APlayerControllerBase_Statics::NewProp_TurnRateGamepad_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_APlayerControllerBase_Statics::NewProp_LookUpRateGamepad_MetaData[] = {
		{ "Category", "PlayerControllerBase" },
		{ "ModuleRelativePath", "PlayerControllerBase/PlayerControllerBase.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_APlayerControllerBase_Statics::NewProp_LookUpRateGamepad = { "LookUpRateGamepad", nullptr, (EPropertyFlags)0x0020080000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(APlayerControllerBase, LookUpRateGamepad), METADATA_PARAMS(Z_Construct_UClass_APlayerControllerBase_Statics::NewProp_LookUpRateGamepad_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_APlayerControllerBase_Statics::NewProp_LookUpRateGamepad_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_APlayerControllerBase_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_APlayerControllerBase_Statics::NewProp_CameraComponent,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_APlayerControllerBase_Statics::NewProp_CameraComponentSecondary,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_APlayerControllerBase_Statics::NewProp_TurnRateGamepad,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_APlayerControllerBase_Statics::NewProp_LookUpRateGamepad,
	};
		const UECodeGen_Private::FImplementedInterfaceParams Z_Construct_UClass_APlayerControllerBase_Statics::InterfaceParams[] = {
			{ Z_Construct_UClass_UDamageinterface_NoRegister, (int32)VTABLE_OFFSET(APlayerControllerBase, IDamageinterface), false },  // 2638534275
			{ Z_Construct_UClass_USteeringinterface_NoRegister, (int32)VTABLE_OFFSET(APlayerControllerBase, ISteeringinterface), false },  // 3367498803
		};
	const FCppClassTypeInfoStatic Z_Construct_UClass_APlayerControllerBase_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<APlayerControllerBase>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_APlayerControllerBase_Statics::ClassParams = {
		&APlayerControllerBase::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_APlayerControllerBase_Statics::PropPointers,
		InterfaceParams,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_APlayerControllerBase_Statics::PropPointers),
		UE_ARRAY_COUNT(InterfaceParams),
		0x009000A4u,
		METADATA_PARAMS(Z_Construct_UClass_APlayerControllerBase_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_APlayerControllerBase_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_APlayerControllerBase()
	{
		if (!Z_Registration_Info_UClass_APlayerControllerBase.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_APlayerControllerBase.OuterSingleton, Z_Construct_UClass_APlayerControllerBase_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_APlayerControllerBase.OuterSingleton;
	}
	template<> GAMECORE_API UClass* StaticClass<APlayerControllerBase>()
	{
		return APlayerControllerBase::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(APlayerControllerBase);
	APlayerControllerBase::~APlayerControllerBase() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_GameCore_Source_GameCore_PlayerControllerBase_PlayerControllerBase_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_GameCore_Source_GameCore_PlayerControllerBase_PlayerControllerBase_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_APlayerControllerBase, APlayerControllerBase::StaticClass, TEXT("APlayerControllerBase"), &Z_Registration_Info_UClass_APlayerControllerBase, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(APlayerControllerBase), 2383233814U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_GameCore_Source_GameCore_PlayerControllerBase_PlayerControllerBase_h_360387300(TEXT("/Script/GameCore"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_GameCore_Source_GameCore_PlayerControllerBase_PlayerControllerBase_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_GameCore_Source_GameCore_PlayerControllerBase_PlayerControllerBase_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS

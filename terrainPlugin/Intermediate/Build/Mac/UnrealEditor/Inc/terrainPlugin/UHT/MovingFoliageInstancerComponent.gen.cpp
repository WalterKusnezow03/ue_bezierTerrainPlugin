// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "terrainPlugin/FoliageInstancing/MovingFoliageInstancerComponent.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeMovingFoliageInstancerComponent() {}
// Cross Module References
	ENGINE_API UClass* Z_Construct_UClass_UActorComponent();
	ENGINE_API UClass* Z_Construct_UClass_UInstancedStaticMeshComponent_NoRegister();
	TERRAINPLUGIN_API UClass* Z_Construct_UClass_UMovingFoliageInstancerComponent();
	TERRAINPLUGIN_API UClass* Z_Construct_UClass_UMovingFoliageInstancerComponent_NoRegister();
	UPackage* Z_Construct_UPackage__Script_terrainPlugin();
// End Cross Module References
	void UMovingFoliageInstancerComponent::StaticRegisterNativesUMovingFoliageInstancerComponent()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UMovingFoliageInstancerComponent);
	UClass* Z_Construct_UClass_UMovingFoliageInstancerComponent_NoRegister()
	{
		return UMovingFoliageInstancerComponent::StaticClass();
	}
	struct Z_Construct_UClass_UMovingFoliageInstancerComponent_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_instancer_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_instancer;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UMovingFoliageInstancerComponent_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UActorComponent,
		(UObject* (*)())Z_Construct_UPackage__Script_terrainPlugin,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UMovingFoliageInstancerComponent_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/// @brief attach this component to ameshactor\n" },
		{ "IncludePath", "FoliageInstancing/MovingFoliageInstancerComponent.h" },
		{ "ModuleRelativePath", "FoliageInstancing/MovingFoliageInstancerComponent.h" },
		{ "ToolTip", "@brief attach this component to ameshactor" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UMovingFoliageInstancerComponent_Statics::NewProp_instancer_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "FoliageInstancing/MovingFoliageInstancerComponent.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UMovingFoliageInstancerComponent_Statics::NewProp_instancer = { "instancer", nullptr, (EPropertyFlags)0x0020080000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UMovingFoliageInstancerComponent, instancer), Z_Construct_UClass_UInstancedStaticMeshComponent_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UMovingFoliageInstancerComponent_Statics::NewProp_instancer_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UMovingFoliageInstancerComponent_Statics::NewProp_instancer_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UMovingFoliageInstancerComponent_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UMovingFoliageInstancerComponent_Statics::NewProp_instancer,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UMovingFoliageInstancerComponent_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UMovingFoliageInstancerComponent>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UMovingFoliageInstancerComponent_Statics::ClassParams = {
		&UMovingFoliageInstancerComponent::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_UMovingFoliageInstancerComponent_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_UMovingFoliageInstancerComponent_Statics::PropPointers),
		0,
		0x00B000A4u,
		METADATA_PARAMS(Z_Construct_UClass_UMovingFoliageInstancerComponent_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UMovingFoliageInstancerComponent_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UMovingFoliageInstancerComponent()
	{
		if (!Z_Registration_Info_UClass_UMovingFoliageInstancerComponent.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UMovingFoliageInstancerComponent.OuterSingleton, Z_Construct_UClass_UMovingFoliageInstancerComponent_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UMovingFoliageInstancerComponent.OuterSingleton;
	}
	template<> TERRAINPLUGIN_API UClass* StaticClass<UMovingFoliageInstancerComponent>()
	{
		return UMovingFoliageInstancerComponent::StaticClass();
	}
	UMovingFoliageInstancerComponent::UMovingFoliageInstancerComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UMovingFoliageInstancerComponent);
	UMovingFoliageInstancerComponent::~UMovingFoliageInstancerComponent() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_FoliageInstancing_MovingFoliageInstancerComponent_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_FoliageInstancing_MovingFoliageInstancerComponent_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UMovingFoliageInstancerComponent, UMovingFoliageInstancerComponent::StaticClass, TEXT("UMovingFoliageInstancerComponent"), &Z_Registration_Info_UClass_UMovingFoliageInstancerComponent, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UMovingFoliageInstancerComponent), 3444097040U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_FoliageInstancing_MovingFoliageInstancerComponent_h_4107760295(TEXT("/Script/terrainPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_FoliageInstancing_MovingFoliageInstancerComponent_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_terrainPlugin_Source_terrainPlugin_FoliageInstancing_MovingFoliageInstancerComponent_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS

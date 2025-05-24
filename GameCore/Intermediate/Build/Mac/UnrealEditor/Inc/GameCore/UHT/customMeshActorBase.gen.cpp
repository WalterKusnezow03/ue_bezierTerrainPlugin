// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "GameCore/MeshGenBase/customMeshActorBase.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodecustomMeshActorBase() {}
// Cross Module References
	ENGINE_API UClass* Z_Construct_UClass_AActor();
	GAMECORE_API UClass* Z_Construct_UClass_AcustomMeshActorBase();
	GAMECORE_API UClass* Z_Construct_UClass_AcustomMeshActorBase_NoRegister();
	PROCEDURALMESHCOMPONENT_API UClass* Z_Construct_UClass_UProceduralMeshComponent_NoRegister();
	UPackage* Z_Construct_UPackage__Script_GameCore();
// End Cross Module References
	void AcustomMeshActorBase::StaticRegisterNativesAcustomMeshActorBase()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(AcustomMeshActorBase);
	UClass* Z_Construct_UClass_AcustomMeshActorBase_NoRegister()
	{
		return AcustomMeshActorBase::StaticClass();
	}
	struct Z_Construct_UClass_AcustomMeshActorBase_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Mesh_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_Mesh;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_MeshNoRaycast_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_MeshNoRaycast;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AcustomMeshActorBase_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AActor,
		(UObject* (*)())Z_Construct_UPackage__Script_GameCore,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AcustomMeshActorBase_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "MeshGenBase/customMeshActorBase.h" },
		{ "ModuleRelativePath", "MeshGenBase/customMeshActorBase.h" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AcustomMeshActorBase_Statics::NewProp_Mesh_MetaData[] = {
		{ "Category", "customMeshActorBase" },
		{ "Comment", "/// @brief saves the mesh data in a map for each layer, keeps things organized\n// std::map<int, MeshData> meshLayersMap;\n// std::map<int, MeshData> meshLayersMapNoRaycast;\n" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "MeshGenBase/customMeshActorBase.h" },
		{ "ToolTip", "@brief saves the mesh data in a map for each layer, keeps things organized\nstd::map<int, MeshData> meshLayersMap;\nstd::map<int, MeshData> meshLayersMapNoRaycast;" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AcustomMeshActorBase_Statics::NewProp_Mesh = { "Mesh", nullptr, (EPropertyFlags)0x00200800000a0009, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(AcustomMeshActorBase, Mesh), Z_Construct_UClass_UProceduralMeshComponent_NoRegister, METADATA_PARAMS(Z_Construct_UClass_AcustomMeshActorBase_Statics::NewProp_Mesh_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_AcustomMeshActorBase_Statics::NewProp_Mesh_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AcustomMeshActorBase_Statics::NewProp_MeshNoRaycast_MetaData[] = {
		{ "Category", "customMeshActorBase" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "MeshGenBase/customMeshActorBase.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AcustomMeshActorBase_Statics::NewProp_MeshNoRaycast = { "MeshNoRaycast", nullptr, (EPropertyFlags)0x00200800000a0009, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(AcustomMeshActorBase, MeshNoRaycast), Z_Construct_UClass_UProceduralMeshComponent_NoRegister, METADATA_PARAMS(Z_Construct_UClass_AcustomMeshActorBase_Statics::NewProp_MeshNoRaycast_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_AcustomMeshActorBase_Statics::NewProp_MeshNoRaycast_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_AcustomMeshActorBase_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AcustomMeshActorBase_Statics::NewProp_Mesh,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AcustomMeshActorBase_Statics::NewProp_MeshNoRaycast,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_AcustomMeshActorBase_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AcustomMeshActorBase>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_AcustomMeshActorBase_Statics::ClassParams = {
		&AcustomMeshActorBase::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_AcustomMeshActorBase_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_AcustomMeshActorBase_Statics::PropPointers),
		0,
		0x009000A4u,
		METADATA_PARAMS(Z_Construct_UClass_AcustomMeshActorBase_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_AcustomMeshActorBase_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_AcustomMeshActorBase()
	{
		if (!Z_Registration_Info_UClass_AcustomMeshActorBase.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AcustomMeshActorBase.OuterSingleton, Z_Construct_UClass_AcustomMeshActorBase_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_AcustomMeshActorBase.OuterSingleton;
	}
	template<> GAMECORE_API UClass* StaticClass<AcustomMeshActorBase>()
	{
		return AcustomMeshActorBase::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(AcustomMeshActorBase);
	AcustomMeshActorBase::~AcustomMeshActorBase() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_GameCore_Source_GameCore_MeshGenBase_customMeshActorBase_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_GameCore_Source_GameCore_MeshGenBase_customMeshActorBase_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_AcustomMeshActorBase, AcustomMeshActorBase::StaticClass, TEXT("AcustomMeshActorBase"), &Z_Registration_Info_UClass_AcustomMeshActorBase, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AcustomMeshActorBase), 337468243U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_GameCore_Source_GameCore_MeshGenBase_customMeshActorBase_h_3216493822(TEXT("/Script/GameCore"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_GameCore_Source_GameCore_MeshGenBase_customMeshActorBase_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_GameCore_Source_GameCore_MeshGenBase_customMeshActorBase_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS

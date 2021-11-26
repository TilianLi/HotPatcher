// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

// project header
#include "HotPatcherLog.h"
#include "FlibPatchParserHelper.h"
#include "HotPatcherLog.h"
#include "Cooker/HotPatcherCookerSettingBase.h"

// engine header
#include "Misc/FileHelper.h"
#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "Engine/EngineTypes.h"
#include "Kismet/KismetStringLibrary.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "FMultiCookerSettings.generated.h"

USTRUCT(BlueprintType)
struct HOTPATCHEREDITOR_API FCookerShaderOptions
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSharedShaderLibrary = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bNativeShader = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooker",meta=(EditCondition="bSharedShaderLibrary"))
	bool bMergeShaderLibrary = false;
};
/** Singleton wrapper to allow for using the setting structur e in SSettingsView */
USTRUCT(BlueprintType)
struct HOTPATCHEREDITOR_API FMultiCookerSettings: public FHotPatcherSettingBase
{
	GENERATED_USTRUCT_BODY()

	FMultiCookerSettings()
	{
		ShaderOptions.bSharedShaderLibrary = true;
		ShaderOptions.bNativeShader = true;
		bSerializeAssetRegistry = true;
		bStorageConfig = true;
		SavePath.Path = TEXT("[PROJECTDIR]/Saved/HotPatcher/MultiCooker");
		bStandaloneMode = false;
	}
public:

	virtual TArray<FDirectoryPath>& GetAssetIncludeFilters()override { return AssetIncludeFilters; };
	virtual TArray<FDirectoryPath>& GetAssetIgnoreFilters()override { return AssetIgnoreFilters; };
	virtual TArray<FPatcherSpecifyAsset>& GetIncludeSpecifyAssets() override {return IncludeSpecifyAssets; };

	bool IsValidConfig()const;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Filter",meta = (RelativeToGameContentDir, LongPackageName))
	TArray<FDirectoryPath> AssetIncludeFilters;
	// Ignore directories in AssetIncludeFilters 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Filter", meta = (RelativeToGameContentDir, LongPackageName))
	TArray<FDirectoryPath> AssetIgnoreFilters;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Filter")
	bool bForceSkipContent = true;
	// force exclude asset folder e.g. Exclude editor content when cooking in Project Settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Filter",meta = (RelativeToGameContentDir, LongPackageName, EditCondition="bForceSkipContent"))
	TArray<FDirectoryPath> ForceSkipContentRules;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Filter",meta = (EditCondition="bForceSkipContent"))
	TArray<FSoftObjectPath> ForceSkipAssets;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Filter")
	bool bIncludeHasRefAssetsOnly = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Filter")
	bool bAnalysisFilterDependencies=true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Filter")
	bool bRecursiveWidgetTree = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Filter",meta = (EditCondition="bAnalysisFilterDependencies"))
	TArray<EAssetRegistryDependencyTypeEx> AssetRegistryDependencyTypes{ EAssetRegistryDependencyTypeEx::Packages };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assets")
	TArray<FPatcherSpecifyAsset> IncludeSpecifyAssets;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooker")
	TArray<ETargetPlatform> CookTargetPlatforms;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooker")
	FCookerShaderOptions ShaderOptions;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooker")
	bool bSerializeAssetRegistry;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooker")
	// FAssetRegistryOptions SerializeAssetRegistryOptions;
	// 
	// support UE4.26 later
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooker")
	FIoStoreSettings IoStoreSettings;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooker",meta=(ClampMin=1,ClampMax=20))
	int32 ProcessNumber = 3;
};

inline bool FMultiCookerSettings::IsValidConfig() const
{
	return (IncludeSpecifyAssets.Num() || AssetIncludeFilters.Num() )&& CookTargetPlatforms.Num();
}

USTRUCT()
struct HOTPATCHEREDITOR_API FSingleCookerSettings:public FHotPatcherCookerSettingBase
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FString MissionName;
	UPROPERTY()
	int32 MissionID;
	UPROPERTY()
	TArray<FAssetDetail> CookAssets;
	// UPROPERTY()
	// TArray<FAssetDetail> FaildAssets;

	UPROPERTY()
	FMultiCookerSettings MultiCookerSettings;
};


USTRUCT()
struct HOTPATCHEREDITOR_API FAssetsCollection
{
	GENERATED_BODY()
	UPROPERTY()
	ETargetPlatform TargetPlatform;
	UPROPERTY()
	TArray<FString> Assets;
};

USTRUCT()
struct HOTPATCHEREDITOR_API FCookerFailedCollection
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FString MissionName;
	UPROPERTY()
	int32 MissionID;
	UPROPERTY()
	TMap<ETargetPlatform,FAssetsCollection> CookFailedAssets;

};

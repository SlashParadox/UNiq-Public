// "Copyright (C) Craig Williams, SlashParadox"

#include "UNImage.h"

#include "SUNImage.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"

UUNImage::UUNImage(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
  , CollectionLerpAlpha(0.0f)
{
}

TSharedRef<SWidget> UUNImage::RebuildWidget()
{
  MyUNImage = SNew(SUNImage);
  MyImage = MyUNImage;
  MyUNImage->SetFlipForRightToLeftFlowDirection(bFlipForRightToLeftFlowDirection);
  
  const UWorld* World = GetWorld();
  if (!World)
  {
    UE_LOG(LogSlate, Error, TEXT("[%s] [%s] No world found"), *FString(__FUNCTION__), *GetNameSafe(this));
    return MyUNImage.ToSharedRef();
  }
  
  // If the world is not yet initialized, bind to it to initialize.
  if (World->bIsWorldInitialized == 0)
  {
    FWorldDelegates::OnPostWorldInitialization.AddUObject(this, &ThisClass::OnWorldInitialized);
  }
  else
  {
    InitializeColorData();
  }
  
  return MyUNImage.ToSharedRef();
}

void UUNImage::SynchronizeProperties()
{
  Super::SynchronizeProperties();

  if (MyUNImage.IsValid())
  {
    MyUNImage->SetCollectionColor(CachedCollectionColor);
  }
}

void UUNImage::ReleaseSlateResources(bool bReleaseChildren)
{
  Super::ReleaseSlateResources(bReleaseChildren);

  MyUNImage.Reset();
  MyImage.Reset();
}

void UUNImage::SetCollectionColorIndex(const FUNParameterCollectionIndex& Index, bool bIsPrimary)
{
  FUNCollectionColorData& ColorData = bIsPrimary ? PrimaryCollectionColor : SecondaryCollectionColor;

  if (ColorData.Index != Index)
    UpdateColorData(ColorData, Index, bIsPrimary);
}

void UUNImage::SetCollectionColorCollection(const UMaterialParameterCollection* Collection, bool bIsPrimary)
{
  FUNCollectionColorData& ColorData = bIsPrimary ? PrimaryCollectionColor : SecondaryCollectionColor;
  
  if (ColorData.Index.Collection != Collection)
  {
    RebindColorData(ColorData, Collection, bIsPrimary);
    ReCacheColorDataColor(ColorData);
    CalculateCachedCollectionColor();
  }
}

void UUNImage::SetCollectionColorSoftCollection(const TSoftObjectPtr<UMaterialParameterCollection>& Collection, bool bIsPrimary)
{
  SetCollectionColorCollection(Collection.LoadSynchronous(), bIsPrimary);
}

void UUNImage::SetCollectionColorName(const FName& ParameterName, bool bIsPrimary)
{
  FUNCollectionColorData& ColorData = bIsPrimary ? PrimaryCollectionColor : SecondaryCollectionColor;

  if (ParameterName != NAME_None && ColorData.Index.ParameterName != ParameterName)
  {
    ReCacheColorDataColor(ColorData, ParameterName);
    CalculateCachedCollectionColor();
  }
}

void UUNImage::SetCollectionLerpAlpha(float Alpha)
{
  CollectionLerpAlpha = Alpha;
  CalculateCachedCollectionColor();
}

void UUNImage::SetCollectionColorIndexBP(FUNParameterCollectionIndex Index, bool bIsPrimary)
{
  SetCollectionColorIndex(Index, bIsPrimary);
}

void UUNImage::SetCollectionColorCollectionBP(UMaterialParameterCollection* Collection, bool bIsPrimary)
{
  SetCollectionColorCollection(Collection, bIsPrimary);
}

void UUNImage::SetCollectionColorSoftCollectionBP(TSoftObjectPtr<UMaterialParameterCollection> Collection, bool bIsPrimary)
{
  SetCollectionColorSoftCollection(Collection, bIsPrimary);
}

void UUNImage::SetCollectionColorNameBP(FName ParameterName, bool bIsPrimary)
{
  SetCollectionColorName(ParameterName, bIsPrimary);
}

void UUNImage::CalculateCachedCollectionColor()
{
  CachedCollectionColor = PrimaryCollectionColor.CachedColor + CollectionLerpAlpha * (SecondaryCollectionColor.CachedColor - PrimaryCollectionColor.CachedColor);

  if (MyUNImage.IsValid())
    MyUNImage->SetCollectionColor(CachedCollectionColor);
}

void UUNImage::UpdateColorData(FUNCollectionColorData& ColorData, const FUNParameterCollectionIndex& Index, bool bIsPrimary)
{
  if (ColorData.Index == Index)
    return;

  if (ColorData.Index.Collection != Index.Collection)
    RebindColorData(ColorData, Index.Collection.LoadSynchronous(), bIsPrimary);

  ReCacheColorDataColor(ColorData, Index.ParameterName);

  CalculateCachedCollectionColor();
}

void UUNImage::ForceUpdateColorData(FUNCollectionColorData& ColorData, bool bIsPrimary)
{
  RebindColorData(ColorData, ColorData.Index.Collection.LoadSynchronous(), bIsPrimary);
  ReCacheColorDataColor(ColorData, ColorData.Index.ParameterName);
}

void UUNImage::RebindColorData(FUNCollectionColorData& ColorData, const UMaterialParameterCollection* Collection, bool bIsPrimary)
{
  if (ColorData.Index.Collection && ColorData.CollectionDelegateHandle.IsValid())
  {
    UMaterialParameterCollectionInstance* OldInstance = GetCollectionInstance(ColorData.Index.Collection.LoadSynchronous());
    if (OldInstance)
      OldInstance->OnVectorParameterUpdated().Remove(ColorData.CollectionDelegateHandle);
  }

  ColorData.CollectionDelegateHandle.Reset();
  ColorData.Index.Collection = Collection;
  
  UMaterialParameterCollectionInstance* NewInstance = GetCollectionInstance(Collection);
  if (NewInstance)
  {
    ColorData.CollectionDelegateHandle = NewInstance->OnVectorParameterUpdated().AddUObject(this, &ThisClass::OnVectorParameterUpdated, bIsPrimary);
  }
}

void UUNImage::ReCacheColorDataColor(FUNCollectionColorData& ColorData) const
{
  const UMaterialParameterCollectionInstance* CollectionInstance = GetCollectionInstance(ColorData.Index.Collection.LoadSynchronous());

  if (!CollectionInstance || !CollectionInstance->GetVectorParameterValue(ColorData.Index.ParameterName, ColorData.CachedColor))
    ColorData.CachedColor = ColorData.Index.Collection ? ColorData.Index.Collection->GetVectorParameterByName(ColorData.Index.ParameterName)->DefaultValue : FLinearColor::White;
}

void UUNImage::ReCacheColorDataColor(FUNCollectionColorData& ColorData, const FName& ParameterName) const
{
  ColorData.Index.ParameterName = ParameterName;
  ReCacheColorDataColor(ColorData);
}

void UUNImage::InitializeColorData()
{
  ForceUpdateColorData(PrimaryCollectionColor, true);
  ForceUpdateColorData(SecondaryCollectionColor, false);
  CalculateCachedCollectionColor();
}

void UUNImage::OnWorldInitialized(UWorld* World, const UWorld::InitializationValues InitializationValues)
{
  FWorldDelegates::OnPostWorldInitialization.RemoveAll(this);

  InitializeColorData();
}

void UUNImage::OnPrimaryParameterUpdated(TPair<FName, FLinearColor> ParameterUpdate)
{
  if (ParameterUpdate.Key == PrimaryCollectionColor.Index.ParameterName)
  {
    PrimaryCollectionColor.CachedColor = ParameterUpdate.Value;
    CalculateCachedCollectionColor();
  }
}

void UUNImage::OnSecondaryParameterUpdated(TPair<FName, FLinearColor> ParameterUpdate)
{
  if (ParameterUpdate.Key == SecondaryCollectionColor.Index.ParameterName)
  {
    SecondaryCollectionColor.CachedColor = ParameterUpdate.Value;
    CalculateCachedCollectionColor();
  }
}

void UUNImage::OnVectorParameterUpdated(TPair<FName, FLinearColor> ParameterUpdate, bool bIsPrimary)
{
  if (bIsPrimary)
    OnPrimaryParameterUpdated(ParameterUpdate);
  else
    OnSecondaryParameterUpdated(ParameterUpdate);
}

UMaterialParameterCollectionInstance* UUNImage::GetCollectionInstance(const UMaterialParameterCollection* Collection) const
{
  if (!Collection)
  {
    return nullptr;
  }
  
  UWorld* World = GetWorld();
  if (!World)
  {
    UE_LOG(LogSlate, Error, TEXT("[%s] [%s] World not found!"), *FString(__FUNCTION__), *GetNameSafe(this));
    return nullptr;
  }
  
  // Check that the world is initialized, and thus the collection instances exist.
  if (World->bIsWorldInitialized == 0)
  {
#if WITH_EDITORONLY_DATA
    if (!IsDesignTime())
    {
      UE_LOG(LogSlate, Warning, TEXT("[%s] [%s] World is not initialized yet!"), *FString(__FUNCTION__), *GetNameSafe(this));
      return nullptr;
    }
#else
    UE_LOG(LogSlate, Warning, TEXT("[%s] [%s] World is not initialized yet!"), *FString(__FUNCTION__), *GetNameSafe(this));
    return nullptr;
#endif
    return nullptr;
  }
  
  // Get the collection instance.
  UMaterialParameterCollectionInstance* Instance = World->GetParameterCollectionInstance(Collection);
  if (!Instance)
  {
    UE_LOG(LogSlate, Warning, TEXT("[%s] [%s] Unable to get instance parameter collection! Collection: [%s]"), *FString(__FUNCTION__), *GetNameSafe(this), *GetNameSafe(Collection));
  }
  
  return Instance;
}
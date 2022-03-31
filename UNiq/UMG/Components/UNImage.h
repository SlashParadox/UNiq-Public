// "Copyright (C) Craig Williams, SlashParadox"

#pragma once

#include "Components/Image.h"
#include "Materials/MaterialParameterCollection.h"

#include "UNImage.generated.h"

class SUNImage;

/**
 * @struct FUNParameterCollectionIndex
 * @brief A struct containing parameter data into a parameter collection.
 */
USTRUCT(BlueprintType)
struct FUNParameterCollectionIndex
{
  GENERATED_BODY()

  FUNParameterCollectionIndex()
    : Collection(nullptr)
  {
  }

  FUNParameterCollectionIndex(const TObjectPtr<UMaterialParameterCollection> InCollection, const FName& InName)
    : Collection(InCollection)
    , ParameterName(InName)
  {
  }

  // The parameter collection to get the color from.
  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  TSoftObjectPtr<UMaterialParameterCollection> Collection;

  // The name of the parameter in the Collection.
  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  FName ParameterName;

  FORCEINLINE bool operator==(const FUNParameterCollectionIndex& Other) const
  {
    return this->ParameterName == Other.ParameterName && this->Collection == Other.Collection; 
  }

  FORCEINLINE bool operator!=(const FUNParameterCollectionIndex& Other) const
  {
    return this->ParameterName != Other.ParameterName || this->Collection != Other.Collection;
  }
};

/**
 * @struct FUNCollectionColorData
 * @brief A struct containing data about a color obtained from a parameter collection.
 */
USTRUCT()
struct FUNCollectionColorData
{
  GENERATED_BODY()

  FUNCollectionColorData()
  {
  }

  // The parameter collection and parameter name associated with this color.
  UPROPERTY(EditAnywhere, meta = (ShowOnlyInnerProperties))
  FUNParameterCollectionIndex Index;

  // The cached color obtained from the parameter collection.
  FLinearColor CachedColor;

  // A handle to the delegate for this color data.
  FDelegateHandle CollectionDelegateHandle;
};

/**
 * @class UUNImage
 * @brief An image widget. This widget listens to changes in a MaterialParameterCollection. It contains
 * two colors, a primary and secondary color. These two can be lerped between for animation purposes.
 */
UCLASS(BlueprintType, Blueprintable)
class UNIQ_API UUNImage : public UImage
{
  GENERATED_UCLASS_BODY()

protected:
  // Begin UWidget Interface
  virtual TSharedRef<SWidget> RebuildWidget() override;
  virtual void SynchronizeProperties() override;
  virtual void ReleaseSlateResources(bool bReleaseChildren) override;
  // End UWidget Interface

public:
  /**
   * Sets the index of a collection color.
   * @param Index The collection and parameter name to use.
   * @param bIsPrimary If true, this sets to the PrimaryCollectionColor. Otherwise, sets to the SecondaryCollectionColor.
   */
  UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Collection Color", DisplayName = "Set Collection Color Index (Const)")
  void SetCollectionColorIndex(const FUNParameterCollectionIndex& Index, bool bIsPrimary);

  /**
   * Sets the parameter collection of a collection color.
   * @param Collection The collection to use.
   * @param bIsPrimary If true, this sets to the PrimaryCollectionColor. Otherwise, sets to the SecondaryCollectionColor.
   */
  UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Collection Color", DisplayName = "Set Collection Color Collection (Const)")
  void SetCollectionColorCollection(const UMaterialParameterCollection* Collection, bool bIsPrimary);

  /**
   * Sets the parameter collection of a collection color.
   * @param Collection The collection to use.
   * @param bIsPrimary If true, this sets to the PrimaryCollectionColor. Otherwise, sets to the SecondaryCollectionColor.
   */
  UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Collection Color", DisplayName = "Set Collection Color Soft Collection (Const)")
  void SetCollectionColorSoftCollection(const TSoftObjectPtr<UMaterialParameterCollection>& Collection, bool bIsPrimary);

  /**
   * Sets the parameter name of a collection color.
   * @param ParameterName The name to use. Make sure it exists in the collection color's collection too!
   * @param bIsPrimary If true, this sets to the PrimaryCollectionColor. Otherwise, sets to the SecondaryCollectionColor.
   */
  UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Collection Color", DisplayName = "Set Collection Color Name (Const)")
  void SetCollectionColorName(const FName& ParameterName, bool bIsPrimary);

  /**
   * Sets the linear interpolation between the primary and secondary collection colors.
   * @param Alpha The linear interpolation alpha to use.
   */
  UFUNCTION(BlueprintCallable, Category = "Collection Color")
  void SetCollectionLerpAlpha(float Alpha);

  /**
   * Gets the linear interpolation between the primary and secondary collection colors.
   * @returns Returns the CollectionLerpAlpha.
   */
  UFUNCTION(BlueprintPure, Category = "Collection Color")
  float GetCollectionLerpAlpha() const { return CollectionLerpAlpha; }

protected:
  /**
   * Sets the index of a collection color.
   * @param Index The collection and parameter name to use.
   * @param bIsPrimary If true, this sets to the PrimaryCollectionColor. Otherwise, sets to the SecondaryCollectionColor.
   */
  UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Collection Color", DisplayName = "Set Collection Color Index")
  void SetCollectionColorIndexBP(FUNParameterCollectionIndex Index, bool bIsPrimary = true);

  /**
   * Sets the parameter collection of a collection color.
   * @param Collection The collection to use.
   * @param bIsPrimary If true, this sets to the PrimaryCollectionColor. Otherwise, sets to the SecondaryCollectionColor.
   */
  UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Collection Color", DisplayName = "Set Collection Color Collection")
  void SetCollectionColorCollectionBP(UMaterialParameterCollection* Collection, bool bIsPrimary = true);

  /**
   * Sets the parameter collection of a collection color.
   * @param Collection The collection to use.
   * @param bIsPrimary If true, this sets to the PrimaryCollectionColor. Otherwise, sets to the SecondaryCollectionColor.
   */
  UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Collection Color", DisplayName = "Set Collection Color Soft Collection")
  void SetCollectionColorSoftCollectionBP(TSoftObjectPtr<UMaterialParameterCollection> Collection, bool bIsPrimary = true);

  /**
   * Sets the parameter name of a collection color.
   * @param ParameterName The name to use. Make sure it exists in the collection color's collection too!
   * @param bIsPrimary If true, this sets to the PrimaryCollectionColor. Otherwise, sets to the SecondaryCollectionColor.
   */
  UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Collection Color", DisplayName = "Set Collection Color Name")
  void SetCollectionColorNameBP(FName ParameterName, bool bIsPrimary);

  /** Calculates a final CollectionColor to apply to the slate widget.*/
  void CalculateCachedCollectionColor();

private:
  /**
   * Updates a color data container with a new index.
   * @param ColorData the color data to update.
   * @param Index The new collection and parameter name to use.
   * @param bIsPrimary If true, this is the primary color collection. Otherwise, it is the secondary collection.
   */
  void UpdateColorData(FUNCollectionColorData& ColorData, const FUNParameterCollectionIndex& Index, bool bIsPrimary);

  /**
   * Forces a color data container to reapply its bindings and cached color.
   * @param ColorData the color data to update.
   * @param bIsPrimary If true, this is the primary color collection. Otherwise, it is the secondary collection.
   */
  void ForceUpdateColorData(FUNCollectionColorData& ColorData, bool bIsPrimary);

  /**
   * Updates a color data container with a new material parameter collection and binds to its changes.
   * @param ColorData the color data to update.
   * @param Collection The new collection to use.
   * @param bIsPrimary If true, this is the primary color collection. Otherwise, it is the secondary collection.
   */
  void RebindColorData(FUNCollectionColorData& ColorData, const UMaterialParameterCollection* Collection, bool bIsPrimary);

  /**
   * Re-caches a color data container with its designated color from its index.
   * @param ColorData the color data to update.
   */
  void ReCacheColorDataColor(FUNCollectionColorData& ColorData) const;

  /**
   * Re-caches a color data container with its designated color from its index.
   * @param ColorData the color data to update.
   * @param ParameterName The new parameter name to use.
   */
  void ReCacheColorDataColor(FUNCollectionColorData& ColorData, const FName& ParameterName) const;

  /** Initializes both the primary and secondary color data with their bindings and color caches.*/
  void InitializeColorData();

  /** A delegate called upon the current world being initialized, allowing to set bindings.*/
  void OnWorldInitialized(UWorld* World, const UWorld::InitializationValues InitializationValues);

  /**
   * A delegate called upon a material parameter collection updating a vector value.
   * @param ParameterUpdate the parameter in the collection that was updated.
   * @param bIsPrimary If true, this parameter is for the primary data. Otherwise, it is for the secondary data.
   */
  void OnVectorParameterUpdated(TPair<FName, FLinearColor> ParameterUpdate, bool bIsPrimary);

  /**
   * A delegate called upon updating the primary color data.
   * @param ParameterUpdate the parameter in the collection that was updated.
   */
  void OnPrimaryParameterUpdated(TPair<FName, FLinearColor> ParameterUpdate);

  /**
   * A delegate called upon updating the secondary color data.
   * @param ParameterUpdate the parameter in the collection that was updated.
   */
  void OnSecondaryParameterUpdated(TPair<FName, FLinearColor> ParameterUpdate);

  /**
   * Gets a material parameter collection's world instance, if available.
   * @param Collection The parameter collection used as a key.
   * @returns Returns the collection's world instance, if available. The world must be initialized!
   */
  UMaterialParameterCollectionInstance* GetCollectionInstance(const UMaterialParameterCollection* Collection) const;

protected:
  // The linear interpolation alpha between the primary and secondary collection colors. Use with animations.
  UPROPERTY(EditAnywhere, Interp, BlueprintReadWrite, BlueprintSetter = SetCollectionLerpAlpha)
  float CollectionLerpAlpha;

  // The slate widget for the UN image.
  TSharedPtr<SUNImage> MyUNImage;
  
private:
  // The primary color obtained from a material parameter collection.
  UPROPERTY(EditAnywhere)
  FUNCollectionColorData PrimaryCollectionColor;

  // A secondary color obtained from a material parameter collection. Useful for animations.
  UPROPERTY(EditAnywhere)
  FUNCollectionColorData SecondaryCollectionColor;

  // The cached off final color being displayed for the collection color.
  FLinearColor CachedCollectionColor;
};
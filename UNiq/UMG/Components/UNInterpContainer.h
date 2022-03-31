// "/Copyright (C) Craig Williams, SlashParadox"

#pragma once

#include "Components/Widget.h"
#include "UNInterpContainer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInterpFloatEvent, float, CurrentValue);

/**
 * @class UUNInterpContainer
 * @brief A simple, formless widget that is solely for use in widget animations. Interp its values
 * to give more freedom on changing user-set values.
 */
UCLASS()
class UNIQ_API UUNInterpContainer : public UWidget
{
  GENERATED_UCLASS_BODY()

public:
  // Begin UWidget class
  virtual TSharedRef<SWidget> RebuildWidget() override;
  // End UWidget class

protected:
  // Begin UWidget class
  virtual void ReleaseSlateResources(bool bReleaseChildren) override;
  // End UWidget class

public:
  /** Sets the FloatValue, and calls an event.*/
  UFUNCTION(BlueprintCallable)
  void SetFloatValue(float Value);

  // A delegate called when FloatValue changes.
  UPROPERTY(BlueprintAssignable)
  FInterpFloatEvent OnFloatValueChanged;

protected:
  // An interpretable FloatValue. Animate this in widgets.
  UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, BlueprintSetter=SetFloatValue)
  float FloatValue;

private:
  // The simplest widget possible to use for a low footprint.
  TSharedPtr<SBox> MyBox;
};
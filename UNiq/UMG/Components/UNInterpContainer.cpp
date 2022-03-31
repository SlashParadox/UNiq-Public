// "Copyright (C) Craig Williams, SlashParadox"

#include "UNInterpContainer.h"

UUNInterpContainer::UUNInterpContainer(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
  , FloatValue(0.0f)
{
}

TSharedRef<SWidget> UUNInterpContainer::RebuildWidget()
{
  MyBox = SNew(SBox);
  return MyBox.ToSharedRef();
}

void UUNInterpContainer::ReleaseSlateResources(bool bReleaseChildren)
{
  Super::ReleaseSlateResources(bReleaseChildren);

  MyBox.Reset();
}

void UUNInterpContainer::SetFloatValue(float Value)
{
  if (FMath::IsNearlyEqual(FloatValue, Value))
    return;

  FloatValue = Value;
  OnFloatValueChanged.Broadcast(FloatValue);
}
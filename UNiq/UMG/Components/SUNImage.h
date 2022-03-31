// "Copyright (C) Craig Williams, SlashParadox"

#pragma once

#include "Widgets/Images/SImage.h"

/**
 * @class SUNImage
 * A slate widget class for a UUNImage.
 */
class UNIQ_API SUNImage : public SImage
{
public:
  SLATE_BEGIN_ARGS(SUNImage)
    : _CollectionColor(FLinearColor::White)
    {
    }

  SLATE_ATTRIBUTE(FSlateColor, CollectionColor)

  SLATE_END_ARGS()

  SUNImage()
  {
    SetCanTick(false);
    bCanSupportFocus = false;
  }

  /** Constructs this widget with InArgs */
  void Construct(const FArguments& InArgs);

public:
  // Begin SWidget Interface
  virtual int32 OnPaint( const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled ) const override;
  // End SWidget Interface
  
public:
  /**
   * Sets the CollectionColor.
   * @param InColor The color to use as the CollectionColor.
   */
  void SetCollectionColor(FLinearColor InColor);

  /**
   * Sets the CollectionColor.
   * @param InColor The color to use as the CollectionColor.
   */
  void SetCollectionColor(const TAttribute<FSlateColor>& InColor);

  /**
   * Sets if the widget should flip for right-to-left flow direction. Make sure to use this when
   * creating this slate widget to keep parity with the SImage class.
   * @param bShouldFlip If true, the widget flips for right-to-left flow direction.
   */
  void SetFlipForRightToLeftFlowDirection(bool bShouldFlip);
  
protected:
  // A color obtained from a material parameter collection.
  TAttribute<FSlateColor> CollectionColor;
};
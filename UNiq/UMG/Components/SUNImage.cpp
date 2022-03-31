// "Copyright (C) Craig Williams, SlashParadox"

#include "SUNImage.h"

#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SUNImage::Construct(const FArguments& InArgs)
{
  /*
	ChildSlot
	[
		// Populate the widget
	];
	*/
}

int32 SUNImage::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
  FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
  const FSlateBrush* ImageBrush = Image.GetImage().Get();

  if ((ImageBrush != nullptr) && (ImageBrush->DrawAs != ESlateBrushDrawType::NoDrawType))
  {
    const bool bIsEnabled = ShouldBeEnabled(bParentEnabled);
    const ESlateDrawEffect DrawEffects = bIsEnabled ? ESlateDrawEffect::None : ESlateDrawEffect::DisabledEffect;

    const FLinearColor FinalColorAndOpacity( InWidgetStyle.GetColorAndOpacityTint() * CollectionColor.Get().GetColor(InWidgetStyle) * ColorAndOpacity.Get().GetColor(InWidgetStyle) * ImageBrush->GetTint( InWidgetStyle ) );

    if (bFlipForRightToLeftFlowDirection && GSlateFlowDirection == EFlowDirection::RightToLeft)
    {
      const FGeometry FlippedGeometry = AllottedGeometry.MakeChild(FSlateRenderTransform(FScale2D(-1, 1)));
      FSlateDrawElement::MakeBox(OutDrawElements, LayerId, FlippedGeometry.ToPaintGeometry(), ImageBrush, DrawEffects, FinalColorAndOpacity);
    }
    else
    {
      FSlateDrawElement::MakeBox(OutDrawElements, LayerId, AllottedGeometry.ToPaintGeometry(), ImageBrush, DrawEffects, FinalColorAndOpacity);
    }
  }

  return LayerId;
}

void SUNImage::SetCollectionColor(FLinearColor InColor)
{
  SetCollectionColor(TAttribute<FSlateColor>(InColor));
}

void SUNImage::SetCollectionColor(const TAttribute<FSlateColor>& InColor)
{
  SetAttribute(CollectionColor, InColor, EInvalidateWidgetReason::Paint);
}

void SUNImage::SetFlipForRightToLeftFlowDirection(bool bShouldFlip)
{
  bFlipForRightToLeftFlowDirection = bShouldFlip;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
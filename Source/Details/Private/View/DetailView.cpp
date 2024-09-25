//// Fill out your copyright notice in the Description page of Project Settings.
//
//
//#include "View/DetailView.h"
//
//#define LOCTEXT_NAMESPACE "UPropertyView"
//
//const FText UDetailView::GetPaletteCategory()
//{
//	return LOCTEXT("Advanced", "Advanced");
//}
//
//void UDetailView::ReleaseSlateResources(bool bReleaseChildren)
//{
//	Super::ReleaseSlateResources(bReleaseChildren);
//
//	MyPropertyView.Reset();
//}
//
//TSharedRef<SWidget> UDetailView::RebuildWidget()
//{
//	MyPropertyView = SNew(SDetailView);
//
//
//	return MyPropertyView.ToSharedRef();
//}
//
//TSharedPtr<SWidget> UDetailView::GetAccessibleWidget() const
//{
//	return MyPropertyView;
//}
//
//#undef LOCTEXT_NAMESPACE
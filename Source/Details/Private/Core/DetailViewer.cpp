// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/DetailViewer.h"


UDetailViewer::UDetailViewer()
{
	Guid = FGuid::NewGuid();

	DetailView = SNew(DETAILS_VIEWER::SDetailView)
		;
}

void UDetailViewer::InitByOptions(FDetailOptions Options)
{
	DetailView->InitByOptions(Options);
}

void UDetailViewer::SetObject(UObject* Object)
{
	//DetailView->SetObject(Object);
}

//// Fill out your copyright notice in the Description page of Project Settings.
//
//
//#include "DetailCore/DetialManger.h"
//#include "DetailCreater/DetailCreater.h"
//
//UDetialManager::UDetialManager()
//{
//}
//
//UDetialManager::~UDetialManager()
//{
//}
//
//UDetialManager* UDetialManager::Get()
//{
//	if (!Instance.IsValid())
//	{
//		Instance = TStrongObjectPtr<UDetialManager>(NewObject<UDetialManager>());
//	}
//
//	return Instance.Get();
//}
//
//UDetailViewer* UDetialManager::CreateDetail(FDetailOptions Options)
//{
//	UDetailViewer* DetailViewer = NewObject<UDetailViewer>(this);
//	DetailViewer->InitByOptions(Options);
//	return DetailViewer;
//}
//
//TStrongObjectPtr<UDetialManager> UDetialManager::Instance = nullptr;

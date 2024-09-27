// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/DetialManger.h"
#include "Creater/DetailCreater.h"
#include "Core/DetailInfo.h"
#include "Misc/LazySingleton.h"

FDetialManager::FDetialManager()
{
}

FDetialManager::~FDetialManager()
{
}

FDetialManager& FDetialManager::Get()
{
	return TLazySingleton<FDetialManager>::Get();
}


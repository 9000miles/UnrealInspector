// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PropertyHolder.h"

FPropertyHolder::FPropertyHolder(TWeakObjectPtr<UObject> Object, UE_Property* Property)
{
	Outermost = Object;
	MyProperty = Property;
}

FPropertyHolder::~FPropertyHolder()
{
}

const FString FPropertyHolder::GetName()
{
	return MyProperty->GetName();
}

const FText FPropertyHolder::GetDisplayName()
{
	return MyProperty->GetDisplayNameText();
}

const FString FPropertyHolder::GetPropertyType()
{
	if (MyProperty == nullptr) return FString();

	return MyProperty->GetCPPType();
}

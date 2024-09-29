// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/SPropertyWidget.h"

namespace DETAILS_VIEWER
{

	void SPropertyWidget::Construct(TSharedPtr<FPropertyInfo> InPropertyInfo)
	{
		PropertyInfo = InPropertyInfo;
	}

}
// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/SPropertyWidget.h"

namespace DetailsViewer
{

	void SPropertyWidget::Construct(TSharedPtr<FPropertyHolder> InPropertyHolder)
	{
		PropertyHolder = InPropertyHolder;
	}

}
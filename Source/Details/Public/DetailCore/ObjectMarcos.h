// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

namespace UM
{
	enum
	{
		PropertyBuilder,//The subclass name inherits from UPropertyBuilder and implements CreatePropertyWidget
		ShowReset,//Whether the reset button is displayed, which defaults to false
		
		FuncPropertyRowWidget,
		DetailRowWidget,//The Details panel defines the Detail Row of custom Widget function names, a member function of this class tagged with UFUNCTION

		CustomDetail,//The custom detail panel for this class instance, fills in the subclass name inherited from UDetailObjectWidget, and implements the CreateWidget function
	};
}
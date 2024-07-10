// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IHackable.generated.h"

/**
 * 
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UIHackable : public UInterface
{
	GENERATED_BODY()
};
class IIHackable
{
	GENERATED_BODY()
	 
public:
	virtual void HandleHacking();
};

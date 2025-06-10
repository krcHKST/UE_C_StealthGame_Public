// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <math.h>
#define ANGLE_DIFFERENCE(value1,value2)(fmodf(((value1) - (value2) + 180.0f),360.0f)  - 180.0f)
/**
 * 
 */
class TOPDOWNGAME_1_API ProjectMacros
{
public:
	ProjectMacros();
	~ProjectMacros();
};

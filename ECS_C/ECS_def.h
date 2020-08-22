#pragma once

#include "Log/Log_def.h"
#include "Entity_def.h"
#include "ECSGlobalEvents_def.h"

typedef struct ECS_TYP
{
	Log_PTR Log;
	ECS_EntityContainer EntityContainer;
	ECS_GlobalEvents GlobalEvents;
} ECS, *ECS_PTR;
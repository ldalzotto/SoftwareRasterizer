#pragma once

#include "v2/_interface/BoxC_def.h"
#include "Objects/Resource/Mesh_def.h"
#include "Component_def.h"

typedef struct MeshRenderer_TYP
{
	ECS_ComponentHeader Header;
	MeshResource_HANDLE MeshResource;
	BoxF MeshBoundingBox;
}MeshRenderer, * MeshRenderer_PTR;
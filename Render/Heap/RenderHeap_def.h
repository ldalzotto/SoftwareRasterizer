#pragma once

#include "Memory/PoolAllocator_def.h"
#include "Objects/Resource/Mesh_def.h"
#include "Objects/Resource/Material_def.h"

typedef struct PoolAllocator_Vertex_TYP
{
	POOLALLOCATOR_TYPE_DEFINITION(Array_Vertex)
}PoolAllocator_Vertex, * PoolAllocator_Vertex_PTR;

typedef struct PoolAllocator_Polygon_TYP
{
	POOLALLOCATOR_TYPE_DEFINITION(Array_Polygon_VertexIndex)
}PoolAllocator_Polygon, * PoolAllocator_Polygon_PTR;

typedef struct PoolAllocator_Material_TYP
{
	POOLALLOCATOR_TYPE_DEFINITION(Array_Material)
}PoolAllocator_Material, * PoolAllocator_Material_PTR;

typedef struct PoolAllocator_PolygonUV_TYP
{
	POOLALLOCATOR_TYPE_DEFINITION(Array_PolygonUV)
}PoolAllocator_PolygonUV, * PoolAllocator_PolygonUV_PTR;

typedef struct RenderHeap_TYP
{
	PoolAllocator_Vertex VertexAllocator;
	PoolAllocator_Polygon PolygonAllocator;
	PoolAllocator_Material MaterialAllocator;
	PoolAllocator_PolygonUV PolygonUVAllocator;
}RenderHeap, * RenderHeap_PTR;

extern RenderHeap RRenderHeap;
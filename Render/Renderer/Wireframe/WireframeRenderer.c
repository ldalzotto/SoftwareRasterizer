#include "WireframeRenderer.h"

#include <math.h>

#include "Heap/RenderHeap.h"
#include "Cull/ObjectCulling.h"
#include "Cull/BackfaceCulling.h"
#include "Objects/Resource/Polygon.h"
#include "v2/_interface/WindowSize.h"
#include "v2/_interface/MatrixC.h"
#include "v2/_interface/VectorStructuresC.h"
#include "Raster/Rasterizer.h"
#include "Renderer/Draw/DrawFunctions.h"
#include "DataStructures/ARRAY.h"
#include "Objects/Resource/Vertex.h"
#include "Objects/Resource/Mesh.h"

ARRAY_ALLOC_FUNCTION(RenderableObjectPipeline, Array_RenderableObjectPipeline_PTR, RenderableObjectPipeline)
ARRAY_PUSHBACKREALLOC_FUNCTION_PTR(RenderableObjectPipeline, Array_RenderableObjectPipeline_PTR, RenderableObjectPipeline)

#if RENDER_PERFORMANCE_TIMER
#include "Clock/Clock.h"

typedef struct PerformanceCounter_TYP
{
	size_t SampleCount;
	TimeClockPrecision AccumulatedTime;
}PerformanceCounter, * PerformanceCounter_PTR;

void PerformanceCounter_PushSample(PerformanceCounter_PTR p_performanceCounter, TimeClockPrecision p_sampleTime);

typedef struct WireframeRendererPerformace_TYP
{
	PerformanceCounter AverageRender;
	PerformanceCounter AverageDataSetup;
	PerformanceCounter AverageLocalToWorld;
	PerformanceCounter AverageBackfaceCulling;
	PerformanceCounter AverageRasterization;
}WireframeRendererPerformace, * WireframeRendererPerformace_PTR;

WireframeRendererPerformace GWireframeRendererPerformace = { 0 };

void WireframeRendererPerformace_Print(WireframeRendererPerformace_PTR p_wireframeRenderPerformance);

#endif

inline void WireframeRenderer_CalculatePixelPosition_FromWorldPosition(VertexPipeline_PTR p_vertexPipeline, size_t p_index, const WireframeRendererInput* p_input)
{
	if (!p_vertexPipeline->PixelPositionCalculated.Memory[p_index])
	{
		Vector4f_PTR p_transformedPosition = &p_vertexPipeline->TransformedPosition.Memory[p_index];

		// World to camera
		Mat_Mul_M4F_V4F(p_input->CameraBuffer->ViewMatrix, p_transformedPosition, &p_vertexPipeline->CameraSpacePosition.Memory[p_index]);

		// Camera to clip
		Mat_Mul_M4F_V4F_Homogeneous(p_input->CameraBuffer->ProjectionMatrix, &p_vertexPipeline->CameraSpacePosition.Memory[p_index], p_transformedPosition);

		// To pixel
		WindowSize_GraphicsAPIToPixel(&p_input->WindowSize, p_transformedPosition->x, p_transformedPosition->y, &p_vertexPipeline->PixelPosition.Memory[p_index].x, &p_vertexPipeline->PixelPosition.Memory[p_index].y);

		p_vertexPipeline->PixelPositionCalculated.Memory[p_index] = 1;
	}
};

void WireframeRenderer_renderV2(const WireframeRendererInput* p_input, Texture3c_PTR p_to, Recti_PTR p_to_clipRect, WireframeRenderer_Memory* p_memory)
{
#if RENDER_PERFORMANCE_TIMER
	TimeClockPrecision l_wireframeRenderBegin = Clock_currentTime_mics();
#endif

	WireframeRenderer_Memory_clear(p_memory, p_to->Width, p_to->Height);
	Vector3c l_wireframeColor = { 255,0,0 };
	Vector4f tmp_vec4_0;

#if RENDER_PERFORMANCE_TIMER
	TimeClockPrecision tmp_timer = Clock_currentTime_mics();
#endif

	for (size_t i = 0; i < p_input->RenderableObjectsBuffer->RenderedObjects.Size; i++)
	{
		RenderedObject_PTR l_renderableObject = p_input->RenderableObjectsBuffer->RenderedObjects.Memory[i];
		Matrix4f l_object_to_camera;
		Mat_Mul_M4F_M4F((Matrix4f_PTR)p_input->CameraBuffer->ViewMatrix, &l_renderableObject->ModelMatrix, &l_object_to_camera);

		if (!ObjectCulled_Boxf(l_renderableObject->MeshBoundingBox, (Matrix4f_PTR)&l_renderableObject->ModelMatrix, (Matrix4f_PTR)&l_object_to_camera, p_input->CameraBuffer->CameraFrustum))
		{
			size_t l_vertexIndexOffset = *p_memory->VertexPipeline.VertexPipelineSize;
			for (size_t j = 0; j < l_renderableObject->Mesh->Vertices.Size; j++)
			{
				Arr_PushBackRealloc_VertexHANDLE(&p_memory->VertexPipeline.Vertex, &l_renderableObject->Mesh->Vertices.Memory[j]);
				Arr_PushBackRealloc_NotInitizlized_Vector4f(&p_memory->VertexPipeline.TransformedPosition);
				Arr_PushBackRealloc_NotInitizlized_Vector4f(&p_memory->VertexPipeline.CameraSpacePosition);
				Arr_PushBackRealloc_NotInitizlized_Vector2i(&p_memory->VertexPipeline.PixelPosition);
				Arr_PushBackRealloc_Char(&p_memory->VertexPipeline.PixelPositionCalculated, 0);
			}

			size_t l_polygonPipelineIndexOffset = p_memory->PolygonPipelines.IsCulled.Size;
			for (size_t j = 0; j < l_renderableObject->Mesh->Polygons.Size; j++)
			{
				Polygon_VertexIndex_PTR l_polygon = &RRenderHeap.PolygonAllocator.array.Memory[l_renderableObject->Mesh->Polygons.Memory[j].Handle];

				Polygon_VertexIndex l_vrticesIndex = 
				{
						l_polygon->v1 + l_vertexIndexOffset,
						l_polygon->v2 + l_vertexIndexOffset,
						l_polygon->v3 + l_vertexIndexOffset
				};

				Arr_PushBackRealloc_Char(&p_memory->PolygonPipelines.IsCulled, 0);
				Arr_PushBackRealloc_Polygon_VertexIndex(&p_memory->PolygonPipelines.VerticesIndex, &l_vrticesIndex);
			}


			RenderableObjectPipeline l_renderaBleObjectPipeline = {
				.RenderedObject = l_renderableObject,
				.PolygonPipelineIndexBeginIncluded = l_polygonPipelineIndexOffset,
				.PolygonPipelineIndexEndExcluded = p_memory->PolygonPipelines.IsCulled.Size,
				.VertexPipelineIndexBeginIncluded = l_vertexIndexOffset,
				.VertexPipelineIndexEndExcluded = *p_memory->VertexPipeline.VertexPipelineSize
			};

			Arr_PushBackRealloc_RenderableObjectPipeline(&p_memory->RederableObjectsPipeline, &l_renderaBleObjectPipeline);

		}
	}

#if RENDER_PERFORMANCE_TIMER
	PerformanceCounter_PushSample(&GWireframeRendererPerformace.AverageDataSetup, Clock_currentTime_mics() - tmp_timer);
#endif

#if RENDER_PERFORMANCE_TIMER
	tmp_timer = Clock_currentTime_mics();
#endif

	// Local to world
	for (size_t i = 0; i < p_memory->RederableObjectsPipeline.Size; i++)
	{
		RenderableObjectPipeline_PTR l_renderableObject = &p_memory->RederableObjectsPipeline.Memory[i];

		for (size_t j = l_renderableObject->VertexPipelineIndexBeginIncluded; j < l_renderableObject->VertexPipelineIndexEndExcluded; j++)
		{
			Vertex_PTR l_vertex = &RRenderHeap.VertexAllocator.array.Memory[p_memory->VertexPipeline.Vertex.Memory[j].Handle];
			Mat_Mul_M4F_V4F(&l_renderableObject->RenderedObject->ModelMatrix, &l_vertex->LocalPosition, &p_memory->VertexPipeline.TransformedPosition.Memory[j]);
		}
	}

#if RENDER_PERFORMANCE_TIMER
	PerformanceCounter_PushSample(&GWireframeRendererPerformace.AverageLocalToWorld, Clock_currentTime_mics() - tmp_timer);
#endif

#if RENDER_PERFORMANCE_TIMER
	tmp_timer = Clock_currentTime_mics();
#endif

	// Backface culling
	for (size_t i = 0; i < *p_memory->PolygonPipelines.PolygonPipelineSize; i++)
	{
		Polygon_VertexIndex_PTR l_polygonVertexIndex = &p_memory->PolygonPipelines.VerticesIndex.Memory[i];
		Polygon4fPTR l_poly =
		{
			.v1 = &p_memory->VertexPipeline.TransformedPosition.Memory[l_polygonVertexIndex->v1],
			.v2 = &p_memory->VertexPipeline.TransformedPosition.Memory[l_polygonVertexIndex->v2],
			.v3 = &p_memory->VertexPipeline.TransformedPosition.Memory[l_polygonVertexIndex->v3]
		};
		p_memory->PolygonPipelines.IsCulled.Memory[i] = BackFaceCulled_Poly4FPTR(&l_poly, &p_input->CameraBuffer->WorldPosition);
	}

#if RENDER_PERFORMANCE_TIMER
	PerformanceCounter_PushSample(&GWireframeRendererPerformace.AverageBackfaceCulling, Clock_currentTime_mics() - tmp_timer);
#endif

#if RENDER_PERFORMANCE_TIMER
	tmp_timer = Clock_currentTime_mics();
#endif

	for (size_t i = 0; i < *p_memory->PolygonPipelines.PolygonPipelineSize; i++)
	{
		Polygon_VertexIndex_PTR l_polygonVertexIndex = &p_memory->PolygonPipelines.VerticesIndex.Memory[i];
		if (!p_memory->PolygonPipelines.IsCulled.Memory[i])
		{
			WireframeRenderer_CalculatePixelPosition_FromWorldPosition(&p_memory->VertexPipeline, l_polygonVertexIndex->v1, p_input);
			WireframeRenderer_CalculatePixelPosition_FromWorldPosition(&p_memory->VertexPipeline, l_polygonVertexIndex->v2, p_input);
			WireframeRenderer_CalculatePixelPosition_FromWorldPosition(&p_memory->VertexPipeline, l_polygonVertexIndex->v3, p_input);

			// Rasterize
			{
				Draw_LineClipped(
					&p_memory->VertexPipeline.PixelPosition.Memory[l_polygonVertexIndex->v1], 
					&p_memory->VertexPipeline.PixelPosition.Memory[l_polygonVertexIndex->v2], 
					&p_memory->RasterizedPixelsBuffer, p_to, p_to_clipRect, &l_wireframeColor);
				Draw_LineClipped(
					&p_memory->VertexPipeline.PixelPosition.Memory[l_polygonVertexIndex->v2],
					&p_memory->VertexPipeline.PixelPosition.Memory[l_polygonVertexIndex->v3],
					&p_memory->RasterizedPixelsBuffer, p_to, p_to_clipRect, &l_wireframeColor);
				Draw_LineClipped(
					&p_memory->VertexPipeline.PixelPosition.Memory[l_polygonVertexIndex->v3],
					&p_memory->VertexPipeline.PixelPosition.Memory[l_polygonVertexIndex->v1],
					&p_memory->RasterizedPixelsBuffer, p_to, p_to_clipRect, &l_wireframeColor);
			}
		}

	}

#if RENDER_PERFORMANCE_TIMER
	PerformanceCounter_PushSample(&GWireframeRendererPerformace.AverageRasterization, Clock_currentTime_mics() - tmp_timer);
	PerformanceCounter_PushSample(&GWireframeRendererPerformace.AverageRender, Clock_currentTime_mics() - l_wireframeRenderBegin);
	WireframeRendererPerformace_Print(&GWireframeRendererPerformace);
#endif
};


void WireframeRenderer_Memory_alloc(WireframeRenderer_Memory* p_memory)
{
	Arr_Alloc_RenderableObjectPipeline(&p_memory->RederableObjectsPipeline, 0);
	
	Arr_Alloc_VertexHANDLE(&p_memory->VertexPipeline.Vertex, 0);
	Arr_Alloc_Vector4f(&p_memory->VertexPipeline.TransformedPosition, 0);
	Arr_Alloc_Vector4f(&p_memory->VertexPipeline.CameraSpacePosition, 0);
	Arr_Alloc_Vector2i(&p_memory->VertexPipeline.PixelPosition, 0);
	Arr_Alloc_Char(&p_memory->VertexPipeline.PixelPositionCalculated, 0);
	p_memory->VertexPipeline.VertexPipelineSize = &p_memory->VertexPipeline.Vertex.Size;
	
	Arr_Alloc_Char(&p_memory->PolygonPipelines.IsCulled, 0);
	Arr_Alloc_Polygon_VertexIndex(&p_memory->PolygonPipelines.VerticesIndex, 0);
	p_memory->PolygonPipelines.PolygonPipelineSize = &p_memory->PolygonPipelines.IsCulled.Size;

	Arr_Alloc_RasterisationStep(&p_memory->RasterizedPixelsBuffer, 0);
};
void WireframeRenderer_Memory_clear(WireframeRenderer_Memory* p_memory, size_t p_width, size_t height)
{
	Arr_Clear(&p_memory->RederableObjectsPipeline.array);

	Arr_Clear(&p_memory->VertexPipeline.Vertex.array);
	Arr_Clear(&p_memory->VertexPipeline.TransformedPosition.array);
	Arr_Clear(&p_memory->VertexPipeline.CameraSpacePosition.array);
	Arr_Clear(&p_memory->VertexPipeline.PixelPosition.array);
	Arr_Clear(&p_memory->VertexPipeline.PixelPositionCalculated.array);

	Arr_Clear(&p_memory->PolygonPipelines.IsCulled.array);
	Arr_Clear(&p_memory->PolygonPipelines.VerticesIndex.array);

	Arr_Clear_RasterisationStep(&p_memory->RasterizedPixelsBuffer);
	Arr_Resize_RasterisationStep(&p_memory->RasterizedPixelsBuffer, p_width > height ? p_width * 2 : height * 2);

};
void WireframeRenderer_Memory_free(WireframeRenderer_Memory* p_memory)
{
	Arr_Free(&p_memory->RederableObjectsPipeline.array);
	
	Arr_Free(&p_memory->VertexPipeline.Vertex.array);
	Arr_Free(&p_memory->VertexPipeline.TransformedPosition.array);
	Arr_Free(&p_memory->VertexPipeline.CameraSpacePosition.array);
	Arr_Free(&p_memory->VertexPipeline.PixelPosition.array);
	Arr_Free(&p_memory->VertexPipeline.PixelPositionCalculated.array);

	Arr_Free(&p_memory->PolygonPipelines.IsCulled.array);
	Arr_Free(&p_memory->PolygonPipelines.VerticesIndex.array);

	Arr_Free_RasterisationStep(&p_memory->RasterizedPixelsBuffer);
};

#if RENDER_PERFORMANCE_TIMER
void PerformanceCounter_PushSample(PerformanceCounter_PTR p_performanceCounter, TimeClockPrecision p_sampleTime)
{
	p_performanceCounter->SampleCount += 1;
	p_performanceCounter->AccumulatedTime += p_sampleTime;
};

TimeClockPrecision PerformanceCounter_GetAverage(PerformanceCounter_PTR p_performanceCounter)
{
	return p_performanceCounter->AccumulatedTime / p_performanceCounter->SampleCount;
};

void WireframeRendererPerformace_Print(WireframeRendererPerformace_PTR p_wireframeRenderPerformance)
{
	printf("WireframeRenderer_renderV2 : %lldmics \n", PerformanceCounter_GetAverage(&p_wireframeRenderPerformance->AverageRender));
	printf("  -> Data Setup %lldmics \n", PerformanceCounter_GetAverage(&p_wireframeRenderPerformance->AverageDataSetup));
	printf("  -> Local To World %lldmics \n", PerformanceCounter_GetAverage(&p_wireframeRenderPerformance->AverageLocalToWorld));
	printf("  -> Backface Culling %lldmics \n", PerformanceCounter_GetAverage(&p_wireframeRenderPerformance->AverageBackfaceCulling));
	printf("  -> Rasterization %lldmics \n", PerformanceCounter_GetAverage(&p_wireframeRenderPerformance->AverageRasterization));
};
#endif
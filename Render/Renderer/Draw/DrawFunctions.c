#include "DrawFunctions.h"

#include "Raster/Rasterizer.h"

#include "Objects/Texture/Texture.h"

void Draw_LineClipped(
	Vector2i_PTR p_begin, Vector2i_PTR p_end,
	ARRAY_RASTERISATIONSTEP_PTR in_out_rasterizedPixelsBuffer,
	Texture3c_PTR p_to, Recti_PTR p_clipRect,
	Vector3c_PTR p_color)
{
	Arr_Clear_RasterisationStep(in_out_rasterizedPixelsBuffer);

	Vector2i l_clippedBegin, l_clippedEnd;
	if (Rasterize_LineClipped(p_begin, p_end, in_out_rasterizedPixelsBuffer, p_clipRect, &l_clippedBegin, &l_clippedEnd))
	{

		char* l_to_memory_cursor = (char*)p_to->Pixels.Memory + Texture_GetElementOffset_3C(l_clippedBegin.x, l_clippedBegin.y, p_to->Width);
		for (size_t j = 0; j < in_out_rasterizedPixelsBuffer->Size; j++)
		{
			if (in_out_rasterizedPixelsBuffer->Memory[j].XDirection == RasterizationStepDirection_ADD)
			{
				l_to_memory_cursor -= sizeof(Vector3c);
			}
			else if (in_out_rasterizedPixelsBuffer->Memory[j].XDirection == RasterizationStepDirection_REMOVE)
			{
				l_to_memory_cursor += sizeof(Vector3c);
			}

			if (in_out_rasterizedPixelsBuffer->Memory[j].YDirection == RasterizationStepDirection_ADD)
			{
				l_to_memory_cursor -= (sizeof(Vector3c) * p_to->Width);
			}
			else if (in_out_rasterizedPixelsBuffer->Memory[j].YDirection == RasterizationStepDirection_REMOVE)
			{
				l_to_memory_cursor += (sizeof(Vector3c) * p_to->Width);
			}
			*(Vector3c_PTR)l_to_memory_cursor = *p_color;
		}
	}
};


void Draw_PolygonClipped(Polygon2i_PTR p_polygon, Array_Vector2i_PTR p_rasterizedPixelBuffer, Texture3c_PTR p_to, Recti_PTR p_clipRect,
	Vector3c_PTR p_color)
{
	// Rasterize_PolygonClipped_DirectTest(p_polygon, p_to, p_clipRect);

	//TODO /!\ Huge performance impact of using the p_rasterizedPixelBuffer.
	Arr_Clear(&p_rasterizedPixelBuffer->array);
	Rasterize_PolygonClipped(p_polygon, p_rasterizedPixelBuffer, p_clipRect);
	for (size_t i = 0; i < p_rasterizedPixelBuffer->Size; i++)
	{
		p_to->Pixels.Memory[p_rasterizedPixelBuffer->Memory[i].x + (p_rasterizedPixelBuffer->Memory[i].y * p_to->Width)] = *p_color;
	}
};

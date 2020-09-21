#include "PixelColorCalculation.h"
#include "Heap/RenderHeap_def.h"
#include <math.h>
#include "Constants.h"
#include "Objects/Resource/Polygon_inline.c"
#include "Objects/Texture/TextureSampling_inline.c"
#include "v2/_interface/VectorC.h"

void PixelColorCaluclation_Polygon_PushCalculations(PolygonPipelineV2_PTR p_polygonPipeline, Vector4f_PTR p_polygonWorldNormal, SolidRenderer_Memory_PTR p_solidRendererMemory)
{
	Material_PTR l_material = &RRenderHeap.MaterialAllocator.array.Memory[p_polygonPipeline->Material.Handle];
	switch (l_material->ShadingType)
	{
	case MATERIAL_SHADING_TYPE_FLAT:
	{
		Arr_PushBackRealloc_Empty_FlatShadingPixelCalculation(&p_solidRendererMemory->FlatShadingCalculations);
		p_solidRendererMemory->FlatShadingCalculations.Memory[p_solidRendererMemory->FlatShadingCalculations.Size - 1] = (FlatShadingPixelCalculation)
		{
			.PolygonFlatNormal = *p_polygonWorldNormal,
			.AttenuatedLightColor = (Color3f){0.0f, 0.0f, 0.0f}
		};

		p_polygonPipeline->FlatShadingCalculationIndex = p_solidRendererMemory->FlatShadingCalculations.Size - 1;

	}
	break;
	}
};

void FlatShadingPixelCalculation_PreCalculation(FlatShadingPixelCalculation_PTR p_flatShadingPixelCalculation, RenderLights_PTR p_renderLights, SolidRenderer_Memory_PTR p_solidRendererMemory)
{
	float l_attenuation = Vec_Dot_3f(&p_renderLights->DirectionalLight.Direction, &p_flatShadingPixelCalculation->PolygonFlatNormal.Vec3) * p_renderLights->DirectionalLight.Intensity;
	// float l_attenuation = (Vec_Dot_3f(&p_renderLights->DirectionalLight.Direction, &p_flatShadingPixelCalculation->PolygonFlatNormal.Vec3) + 1.0f) * 0.5f * p_renderLights->DirectionalLight.Intensity;
	if (l_attenuation <= FLOAT_TOLERANCE) { l_attenuation = 0.0f; }

	Vec_Mul_3f_1f(&p_renderLights->DirectionalLight.Color.Vec, l_attenuation, &p_flatShadingPixelCalculation->AttenuatedLightColor.Vec);
};

void FlatShadingPixelCalculation_ShadePixelColor(
	const FlatShadingPixelCalculation_PTR p_flatShadingPixelCalculation, const Polygon2f_PTR p_polygonUV,
	RenderLights_PTR p_renderLights, Material_PTR p_material,
	const PolygonRasterizer_InterpolationFactor_PTR p_interpolationFactors, Color3f_PTR out_pixelColor)
{

	switch (p_material->MeshPropertyUsage)
	{
	case MATERIAL_MESHPROPERTY_USAGE_UV:
	{
		Vector2f l_interpolatedUv;
		Color3f l_sampledPoint;

		//&RRenderHeap.PolygonUVAllocator.array.Memory[p_polygonPipeline->MaterialMeshProperties.PolygonUV.Handle]
		_i_Polygon_Interpolate_V2F(p_polygonUV, p_interpolationFactors->I0, p_interpolationFactors->I1, p_interpolationFactors->I2, &l_interpolatedUv);

		_i_TextureSample_Point_3f(&RRenderHeap.Texture3cAllocator.array.Memory[p_material->DiffuseTexture.Handle], &l_interpolatedUv, &l_sampledPoint);

		out_pixelColor->Vec.x = (p_material->BaseColor.r * l_sampledPoint.r);
		out_pixelColor->Vec.y = (p_material->BaseColor.g * l_sampledPoint.g);
		out_pixelColor->Vec.z = (p_material->BaseColor.b * l_sampledPoint.b);
	}
	break;
	default:
	{
		out_pixelColor->Vec.x = (p_material->BaseColor.r * p_flatShadingPixelCalculation->AttenuatedLightColor.r) + p_renderLights->AmbientLight.Color.r;
		out_pixelColor->Vec.y = (p_material->BaseColor.g * p_flatShadingPixelCalculation->AttenuatedLightColor.g) + p_renderLights->AmbientLight.Color.g;
		out_pixelColor->Vec.z = (p_material->BaseColor.b * p_flatShadingPixelCalculation->AttenuatedLightColor.b) + p_renderLights->AmbientLight.Color.b;
	}
	break;
	}

};
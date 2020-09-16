#include "PixelColorCalculation.h"
#include "Heap/RenderHeap_def.h"
#include <math.h>
#include "Constants.h"
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
			.Out_AttenuatedLightColor = (Color3f){0.0f, 0.0f, 0.0f}
		};

		p_polygonPipeline->FlatShadingCalculationIndex = p_solidRendererMemory->FlatShadingCalculations.Size - 1;
		
	}
	break;
	}
};

void FlatShadingPixelCalculation_Calculate(FlatShadingPixelCalculation_PTR p_flatShadingPixelCalculation, DirectionalLight_PTR p_directionalLight, AmbientLight_PTR p_ambiantLight, 
	SolidRenderer_Memory_PTR p_solidRendererMemory)
{
	float l_attenuation = Vec_Dot_3f(&p_directionalLight->Direction, &p_flatShadingPixelCalculation->PolygonFlatNormal.Vec3) * p_directionalLight->Intensity;
	if (l_attenuation <= FLOAT_TOLERANCE) { l_attenuation = 0.0f; }
	// float l_attenuation = ((Vec_Dot_3f(&p_directionalLight->Direction, &p_flatShadingPixelCalculation->PolygonFlatNormal.Vec3) + 1.0f) * 0.5f) * p_directionalLight->Intensity;
	// l_attenuation = (l_attenuation + 1.0f) * 0.5f;

	Vec_Mul_3f_1f(&p_directionalLight->Color.Vec, l_attenuation, &p_flatShadingPixelCalculation->Out_AttenuatedLightColor.Vec);

	p_flatShadingPixelCalculation->Out_AmbientColor = p_ambiantLight->Color;
	
	// Vec_Add_3f_3f(&p_flatShadingPixelCalculation->Out_AttenuatedLightColor.Vec, &p_ambiantLight->Color.Vec, &p_flatShadingPixelCalculation->Out_AttenuatedLightColor.Vec);
};

void FlatShadingPixelCalculation_ShadeColor(FlatShadingPixelCalculation_PTR p_flatShadingPixelCalculation, Color3f_PTR p_color, Color3f_PTR out_color)
{
	Vec_Mul_3f_3f(&p_color->Vec, &p_flatShadingPixelCalculation->Out_AttenuatedLightColor.Vec, &out_color->Vec);
	Vec_Add_3f_3f(&out_color->Vec, &p_flatShadingPixelCalculation->Out_AmbientColor.Vec, &out_color->Vec);
};
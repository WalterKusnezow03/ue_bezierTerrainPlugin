#include "WKGlassShader.h"

#if WITH_EDITOR

/// @brief override this method to add more inputs.
void UWKGlassShader::SetupInputsOnConstruct(){
    UMaterial* OuterMaterial = GetTypedOuter<UMaterial>(); //expression must be owned by material itself.
    if(OuterMaterial && !customExpression)
    {
        customExpression = NewObject<UMaterialExpressionCustom>(
            this,
            UMaterialExpressionCustom::StaticClass(),
            NAME_None,
            RF_Transactional
        );
    }
    if(customExpression){
        customExpression->Inputs.Add(FCustomInput{TEXT("Normal")});
        customExpression->Inputs.Add(FCustomInput{TEXT("ViewDir")});
        customExpression->Inputs.Add(FCustomInput{TEXT("IOR")});
    }
    
}

void UWKGlassShader::SetupOutputsOnConstruct(){
    //empty for derivation override

    // OutputType: Float4 für Color
    if(customExpression){
        customExpression->OutputType = CMOT_Float4;
    }
    
}


void UWKGlassShader::SetupInternalExpressionsOnConstruct()
{
    if (customExpression)
    {
        // //epsilon = 0.01;
        customExpression->Code = TEXT(R"(
            
            float3 N = normalize(Normal);
            float3 V = normalize(ViewDir);


            // -- FresnelSchlick --
            float cosTheta = saturate(dot(N, -V));
            float F0 = 0.04;
            float fresnel = F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);

            // -- refraction direction --
            float cosi = clamp(dot(-V, N), -1.0, 1.0);
            float etai = 1.0;
            float etat = IOR;
            float3 n = N;

            if (cosi > 0)
            {
                float tmp = etai;
                etai = etat;
                etat = tmp;
                n = -N;
            }

            float etaRatio = etai / etat;
            float k = 1.0 - etaRatio * etaRatio * (1.0 - cosi * cosi);
            float3 refrDir = (k < 0) ? float3(0,0,0) : etaRatio * V + (etaRatio * cosi - sqrt(k)) * n;

            // -- output --
            return float4(refrDir * fresnel, 1.0);
        )");
    }
}

int32 UWKGlassShader::Compile(FMaterialCompiler *Compiler, int32 outputIndex){
    if(customExpression){
        return customExpression->Compile(Compiler, outputIndex);
    }
    return INDEX_NONE;
}


#endif




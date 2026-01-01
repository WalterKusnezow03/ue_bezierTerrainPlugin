#include "WKGrassShader.h"


#if WITH_EDITOR

/// @brief override this method to add more inputs.
void UWKGrassShader::SetupInputsOnConstruct(){
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
        customExpression->Inputs.Add(FCustomInput{TEXT("vertex")});
        customExpression->Inputs.Add(FCustomInput{TEXT("gridstep")});
        customExpression->Inputs.Add(FCustomInput{TEXT("height")});

        customExpression->Inputs.Add(FCustomInput{TEXT("time")});
        customExpression->Inputs.Add(FCustomInput{TEXT("intervall")});
        customExpression->Inputs.Add(FCustomInput{TEXT("offsetDir")});
    }
    
}

void UWKGrassShader::SetupOutputsOnConstruct(){
    //empty for derivation override

    // OutputType: Float4 für Color
    if(customExpression){
        customExpression->OutputType = CMOT_Float3;
    }
    
}


void UWKGrassShader::SetupInternalExpressionsOnConstruct(){
    //empty for derivation override (using internal expressions of unreal to hide inside)

    // HLSL Code
    if(customExpression){
        customExpression->Code = TEXT(R"(

            // ---- grass shape ----
            if(gridstep <= 0.0){
                gridstep = 10; //einfach so.
            }

            float stepFrac = 1 / gridstep;

            float2 uvMod;
            uvMod.x = fmod(abs(vertex.x) * stepFrac, 1.0); //normalize to 0..1
            uvMod.y = fmod(abs(vertex.y) * stepFrac, 1.0); 

            //bend inwards weigth
            float2 weight1 = float2(1,1) - uvMod;
            float2 weight2 = uvMod - float2(0,0);
            float weight11 = weight1.x + weight1.y;
            float weight22 = weight2.x + weight2.y;
            float maxWeight = cos(max(weight11 / 2.0 , weight22 / 2.0));
            float maskAdd = maxWeight * height;


            float maskX = step(uvMod.x, stepFrac);
            float maskY = step(uvMod.y, stepFrac);
            float mask  = max(maskX, maskY);

            float3 outVertex = float3(0,0,0);
            outVertex.z += (height * mask + maskAdd) / 2;

            // ---- move with time ----
            intervall = abs(intervall);
            if(intervall < 1.0){
                intervall = 1.0;
            }
            intervall = 1.0 / intervall; //logic fix for external time passing

            float timemod = sin(time * intervall);
            float2 timeMask = offsetDir * timemod;
            

            outVertex.x += timeMask.x;
            outVertex.y += timeMask.y;


            return outVertex;
            
        )");
    }
    
}

int32 UWKGrassShader::Compile(FMaterialCompiler *Compiler, int32 outputIndex){
    if(customExpression){
        return customExpression->Compile(Compiler, outputIndex);
    }
    return INDEX_NONE;
}

#endif
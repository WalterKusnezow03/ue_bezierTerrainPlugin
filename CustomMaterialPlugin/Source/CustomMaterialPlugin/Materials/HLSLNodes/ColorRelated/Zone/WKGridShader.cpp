#include "WKGridShader.h"

#if WITH_EDITOR

/// @brief override this method to add more inputs.
void UWKGridShader::SetupInputsOnConstruct(){
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
        customExpression->Inputs.Add(FCustomInput{TEXT("ColorIn")});
        customExpression->Inputs.Add(FCustomInput{TEXT("UVin")});
        customExpression->Inputs.Add(FCustomInput{TEXT("sizeSquare")});
    }
    
}

void UWKGridShader::SetupOutputsOnConstruct(){
    //empty for derivation override

    // OutputType: Float4 für Color
    if(customExpression){
        customExpression->OutputType = CMOT_Float4;
    }
    
}


void UWKGridShader::SetupInternalExpressionsOnConstruct()
{
    if (customExpression)
    {
        // //epsilon = 0.01;
        customExpression->Code = TEXT(R"(
            if(sizeSquare <= 0.0){
                sizeSquare = 10; //einfach so.
            }

            float sizeSquareFlip = 1 / sizeSquare;


            float2 uvMod = fmod(UVin * sizeSquareFlip, 1.0); 

            
            float lineThickness = 0.05;

            //step(a,b): //0 oder 1 wenn a < b
            float inLine = step(uvMod.x, lineThickness) + step(uvMod.y, lineThickness); 
            inLine = saturate(inLine); //clamp auf 0 bis 1


            //lol

            //Color output
            float4 BaseColor = float4(0, 0, 0, 0);
            float4 color = lerp(BaseColor, float4(ColorIn, 1.0), inLine);
            return color;
        )");
    }
}

int32 UWKGridShader::Compile(FMaterialCompiler *Compiler, int32 outputIndex){
    if(customExpression){
        return customExpression->Compile(Compiler, outputIndex);
    }
    return INDEX_NONE;
}







#endif
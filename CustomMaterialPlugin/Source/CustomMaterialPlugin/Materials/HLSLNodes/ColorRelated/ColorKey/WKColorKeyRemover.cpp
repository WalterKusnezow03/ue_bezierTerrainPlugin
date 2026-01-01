#include "WKColorKeyRemover.h"


#if WITH_EDITOR

/// @brief override this method to add more inputs.
void UWKColorKeyRemover::SetupInputsOnConstruct(){
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
        customExpression->Inputs.Add(FCustomInput{TEXT("ColorA")});
        customExpression->Inputs.Add(FCustomInput{TEXT("ColorB")});
    }
    
}

void UWKColorKeyRemover::SetupOutputsOnConstruct(){
    //empty for derivation override

    // OutputType: Float4 für Color
    if(customExpression){
        customExpression->OutputType = CMOT_Float1;
    }
    
}


void UWKColorKeyRemover::SetupInternalExpressionsOnConstruct(){
    //empty for derivation override (using internal expressions of unreal to hide inside)

    // HLSL Code
    if(customExpression){
        customExpression->Code = TEXT(R"(
            // ColorA und ColorB kommen von den angeschlossenen Expressions
            float deltaR = abs(ColorA.r - ColorB.r);
            float deltaG = abs(ColorA.g - ColorB.g);
            float deltaB = abs(ColorA.b - ColorB.b);

            float epsilon = 0.0001;
            if(deltaR < epsilon || deltaG < epsilon || deltaB < epsilon)
                return 0.0;
            else
                return 1.0;
        )");
    }
    
}

int32 UWKColorKeyRemover::Compile(FMaterialCompiler *Compiler, int32 outputIndex){
    if(customExpression){
        return customExpression->Compile(Compiler, outputIndex);
    }
    return INDEX_NONE;
}

#endif
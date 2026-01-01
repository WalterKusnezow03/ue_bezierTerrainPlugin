#include "WKRadialPixelShader.h"

#if WITH_EDITOR
void UWKRadialPixelShader::SetupInputsOnConstruct(){
    colorInputIndex = 0;
    FExpressionInput colorInput;
    colorInput.InputName = FName(TEXT("MixColorInput"));
    expressionInputArray.Add(colorInput);
}

int32 UWKRadialPixelShader::InputColor(FMaterialCompiler *Compiler){
    bool ok = false;
    int32 inputColor = CompileInput(Compiler, colorInputIndex, ok);
    if(ok && inputColor != INDEX_NONE){
        return inputColor;
    }
    return MakeConstant3D(Compiler, 0.0f, 0.0f, 0.0f);
}

int32 UWKRadialPixelShader::ValidMixScalar(FMaterialCompiler *Compiler, float scalar){
    float clamped = FMath::Clamp(scalar, 0.0f, 1.0f);
    return MakeConstant(Compiler, clamped);
}

int32 UWKRadialPixelShader::CompilePixelShader(FMaterialCompiler *Compiler){

    int32 ColorIn = InputColor(Compiler);
    int32 colorWeighted = ValidMixScalar(Compiler, mixScalar);

    int32 radialWeight = RadialScalarFromScreenCoordinateRelativeToCenter(Compiler);

    int32 weightAccumulated = Compiler->Mul(colorWeighted, radialWeight);

    return Compiler->Mul(ColorIn, weightAccumulated);
}

#endif
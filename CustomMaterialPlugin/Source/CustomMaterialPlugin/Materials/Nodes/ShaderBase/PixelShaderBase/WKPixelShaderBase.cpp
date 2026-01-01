#include "WKPixelShaderBase.h"


#if WITH_EDITOR
// --- output ---
void UWKPixelShaderBase::SetupOutputsOnConstruct(){
    Super::SetupOutputsOnConstruct();
    FExpressionOutput pixelOutput;
    pixelOutput.OutputName = FName(TEXT("PixelColorOutput"));
    expressionOutputArray.Add(pixelOutput); 
}



// --- hepers ---
int32 UWKPixelShaderBase::GetScreenPositionAsPixel(FMaterialCompiler* Compiler)
{
    return Compiler->GetPixelPosition();
}

int32 UWKPixelShaderBase::GetScreenPositionAsUV(FMaterialCompiler *Compiler){
    return Compiler->GetViewportUV();
}

int32 UWKPixelShaderBase::GetScreenPoisitionAsUvRelativeToCenter(FMaterialCompiler *Compiler){
    int32 center = MakeConstant2D(Compiler, 0.5f, 0.5f);
    int32 pos = GetScreenPositionAsUV(Compiler);
    //AB = B - A --> center to screen
    return Compiler->Sub(pos, center);
}

int32 UWKPixelShaderBase::RadialScalarFromScreenCoordinateRelativeToCenter(FMaterialCompiler *Compiler){
    int32 uvRelativeToCenter = GetScreenPoisitionAsUvRelativeToCenter(Compiler); //between 0 and 0.5
    int32 uvRelativeToCenterNormalized = Compiler->Mul(
        uvRelativeToCenter, 
        MakeConstant(Compiler, 2.0f) //to make between 0 and 1 scaled
    );

    int32 X = Compiler->ComponentMask(uvRelativeToCenterNormalized, true, false, false, false);
    int32 Y = Compiler->ComponentMask(uvRelativeToCenterNormalized, false, true, false, false);

    //size vector between 0 and 1
    return SizeVector2D(Compiler, X, Y);
}


// -- color extract --
TArray<int32> UWKPixelShaderBase::SplitRGBA(FMaterialCompiler *Compiler, int32 someColor){
    TArray<int32> outComponents;
    outComponents.SetNum(4);

    outComponents[0] = Compiler->ComponentMask(someColor, true, false, false, false);
    outComponents[1] = Compiler->ComponentMask(someColor, false, true, false, false);
    outComponents[2] = Compiler->ComponentMask(someColor, false, false, true, false);
    outComponents[3] = Compiler->ComponentMask(someColor, false, false, false, true);
    return outComponents;
}

int32 UWKPixelShaderBase::MakeRGBA(FMaterialCompiler *Compiler, TArray<int32> &array){
    if(array.Num() == 4){
        MakeConstant4D(
            Compiler, 
            array[0],
            array[1],
            array[2],
            array[3]
        );
    }
    return MakeConstant4D(Compiler, 0.0f, 0.0f, 0.0f, 0.0f);
}

// --- compile ---
int32 UWKPixelShaderBase::Compile(FMaterialCompiler *Compiler, int32 OutputIndex){
    return CompilePixelShader(Compiler);
}

//called by compile
int32 UWKPixelShaderBase::CompilePixelShader(FMaterialCompiler *Compiler){
    return INDEX_NONE;
}

#endif
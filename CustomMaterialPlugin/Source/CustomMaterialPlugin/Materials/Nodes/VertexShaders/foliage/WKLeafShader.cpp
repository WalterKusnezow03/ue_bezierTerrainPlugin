#include "WKLeafShader.h"


#if WITH_EDITOR


// --- input output construct ---

void UWKLeafShader::SetupInputsOnConstruct(){
    Super::SetupInputsOnConstruct();

    /*
    write from cpp:
    // Float-Parameter setzen
    UMaterialInstanceDynamic* dynamic = UMaterialInstanceDynamic::Create(BaseMaterial, this);
    dynamic->SetScalarParameterValue(FName("LowerZValueBounds_float"), 10.0f);
    dynamic->SetScalarParameterValue(FName("HigherZValueBounds_float"), 50.0f);
    */
    FExpressionInput lowZInput;
    lowZInput.InputName = FName(TEXT("LowerZValueBounds_float"));
    expressionInputArray.Add(lowZInput);

    FExpressionInput highZInput;
    highZInput.InputName = FName(TEXT("HigherZValueBounds_float"));
    expressionInputArray.Add(highZInput);
}

void UWKLeafShader::SetupOutputsOnConstruct(){
    Super::SetupOutputsOnConstruct();
    FExpressionOutput vertexOutput;
    vertexOutput.OutputName = FName(TEXT("VertexOutput"));
    expressionOutputArray.Add(vertexOutput);
}

// --- input output construct end ---

FString UWKLeafShader::NodeName() const {
    return TEXT("UWKLeafShader");
}




/// --- input read ---
int32 UWKLeafShader::LowerZ(FMaterialCompiler *Compiler){
    bool ok = true;
    int32 result = CompileInput(Compiler, inputLowLayer, ok);
    if(ok){
        return result;
    }
    return MakeConstant(Compiler, 0.0f);
}

int32 UWKLeafShader::HigherZ(FMaterialCompiler *Compiler){
    bool ok = true;
    int32 result = CompileInput(Compiler, inputHighZLayer, ok);
    if(ok){
        return result;
    }
    return MakeConstant(Compiler, 1000.0f); //10m als max wert (?)
}

int32 UWKLeafShader::RangeZ(FMaterialCompiler *Compiler){
    int32 DeltaZ = Compiler->Max(
        Compiler->Sub(HigherZ(Compiler), LowerZ(Compiler)), 
        Compiler->Constant(1.0f)
    );
    return DeltaZ;
}

int32 UWKLeafShader::LocalInRangeAsScalar(FMaterialCompiler *Compiler, int32 zLocal){
    int32 zLocalRelativeToLower = Compiler->Sub(zLocal, LowerZ(Compiler)); // AB = B - A
    //scalar = dist target / dist all
    int32 zAll = RangeZ(Compiler);
    int32 scalar = Compiler->Div(zLocalRelativeToLower, zAll);
    int32 scalarClamp = Compiler->Clamp(
        scalar,
        MakeConstant(Compiler, 0.00001f),
        MakeConstant(Compiler, 1.0f)
    );

    /*
    //as 1 over X
    int32 inverse = Compiler->Sub(MakeConstant(Compiler, 1.0f), scalarClamp);
    int32 oneOverX = Compiler->Div(MakeConstant(Compiler, 1.0f), inverse);

    int32 oneOverXCapped = Compiler->Clamp(
        scalar,
        MakeConstant(Compiler, 0.00001f),
        MakeConstant(Compiler, 1.0f)
    );*/

    // optional: exponentiell stärker oben, aber nie unendlich
    int32 shapedScalar = Compiler->Mul(scalarClamp, scalarClamp);

    return shapedScalar;
}

/// --- compile ---

int32 UWKLeafShader::Compile(FMaterialCompiler *Compiler, int32 OutputIndex){
    return CompileVertexShader(Compiler);
}

int32 UWKLeafShader::CompileVertexShader(FMaterialCompiler *Compiler){

    //side wise expand?
    int32 local = GetLocalPosition(Compiler);
    int32 local_z = Compiler->ComponentMask(local, false, false, true, false); // x,y,z,h

    int32 strengthDistortion = LocalInRangeAsScalar(Compiler, local_z);
    int32 DistortionScalar = Compiler->Mul(
        MakeConstant(Compiler, 100.0f), //distortion x and y over z range
        strengthDistortion
    );

    //timed
    int32 sinTime = Compiler->Sine(ScaledTime(Compiler, IntervallSlowDown));
    //make pos only
    int32 sinTimeAbs = Compiler->Add(sinTime, MakeConstant(Compiler, 1.0f));
    int32 sinTimeNormalized = Compiler->Mul(sinTimeAbs, MakeConstant(Compiler, 0.5f));
    float wobbleAllowBack = -0.1f;
    int32 sinTimeNormalizedWobble = Compiler->Add(sinTimeNormalized, MakeConstant(Compiler, wobbleAllowBack));
    
    
    int32 DistortTimed = Compiler->Mul(DistortionScalar, sinTimeNormalizedWobble);

    //output as vertex offset on x and y
    int32 windDir = MakeVector2D(
        Compiler,
        MakeConstant(Compiler, 100),
        MakeConstant(Compiler, 100)
    );
    int32 windDirNormalized = Compiler->Normalize(windDir);

    int32 result = Compiler->Mul(windDirNormalized, DistortTimed);

    int32 outVector = MakeVector3D(
        Compiler,
        Compiler->ComponentMask(result, true, false, false, false), 
        Compiler->ComponentMask(result, false, true, false, false),
        MakeConstant(Compiler, 0.0f)
    );
    return outVector;
}

#endif
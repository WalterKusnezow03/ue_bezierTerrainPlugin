#pragma once

#include "CustomMaterialPlugin/Materials/Nodes/ShaderBase/vertexShaderBase/WKVertexShaderBase.h"
#include "WKLeafShader.generated.h"


UCLASS(collapsecategories, hidecategories=Object)
class CUSTOMMATERIALPLUGIN_API UWKLeafShader : public UWKVertexShaderBase
{
    GENERATED_BODY()
public:
    
    /*
    inputs: 
    LowerZValueBounds_float
    HigherZValueBounds_float
    
    
    */



    /// ---- Changable attributes of the water ----
 
    UPROPERTY(EditAnywhere, Category=UWKWaterShader)
    float Amplitude = 10.0f;

    UPROPERTY(EditAnywhere, Category=UWKWaterShader)
    float IntervallSlowDown = 1.0f; 


#if WITH_EDITOR
    virtual void SetupInputsOnConstruct() override;
    virtual void SetupOutputsOnConstruct() override;


    virtual FString NodeName() const override;

    int32 vertexOutputLayer = 0;
    int32 inputLowLayer = 0;
    int32 inputHighZLayer = 1;

    virtual int32 Compile(FMaterialCompiler *Compiler, int32 OutputIndex) override;
    int32 CompileVertexShader(FMaterialCompiler *Compiler);

    //input read
    int32 LowerZ(FMaterialCompiler *Compiler);
    int32 HigherZ(FMaterialCompiler *Compiler);
    int32 RangeZ(FMaterialCompiler *Compiler);
    int32 LocalInRangeAsScalar(FMaterialCompiler *Compiler, int32 zLocal);

#endif
};
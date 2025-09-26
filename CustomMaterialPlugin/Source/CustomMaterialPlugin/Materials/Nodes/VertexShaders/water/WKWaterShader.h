#pragma once

#include "CoreMinimal.h"
#include "Materials/MaterialExpression.h"
#include "MaterialCompiler.h"

#include "CustomMaterialPlugin/Materials/Nodes/ShaderBase/vertexShaderBase/WKVertexShaderBase.h"

#include "WKWaterShader.generated.h"


UCLASS(collapsecategories, hidecategories=Object)
class CUSTOMMATERIALPLUGIN_API UWKWaterShader : public UWKVertexShaderBase
{
    GENERATED_BODY()

protected:
    virtual void SetupOutputsOnConstruct() override;

    int32 vertexOutputLayer = 0;
    int32 normalOutputLayer = 1;


public:
    virtual FString GetDescription() const override;

    /// ---- Changable attributes of the water ----
 
    UPROPERTY(EditAnywhere, Category=UWKWaterShader)
    float Amplitude = 10.0f;

    UPROPERTY(EditAnywhere, Category=UWKWaterShader)
    float IntervallSlowDown = 1.0f; 

    UPROPERTY(EditAnywhere, Category=UWKWaterShader)
    float DistanceBetweenWaves = 500.0f; //every 1 meter

#if WITH_EDITOR

    //convert to layer codes
    int32 AmplitudeCode(FMaterialCompiler *Compiler);

    int32 Frequency(FMaterialCompiler *Compiler);
    int32 RunTime(FMaterialCompiler *Compiler);
    int32 Speed(FMaterialCompiler *Compiler);

    virtual FString NodeName() const override;

    virtual int32 Compile(FMaterialCompiler *Compiler, int32 OutputIndex) override;

    int32 CompileVertexShader(FMaterialCompiler *Compiler);
    int32 CompileNormalShader(FMaterialCompiler *Compiler);
#endif
};
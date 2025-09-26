#pragma once

#include "CustomMaterialPlugin/Materials/Nodes/ShaderBase/PixelShaderBase/WKPixelShaderBase.h"
#include "WKRadialPixelShader.generated.h"


UCLASS(collapsecategories, hidecategories=Object)
class CUSTOMMATERIALPLUGIN_API UWKRadialPixelShader : public UWKPixelShaderBase
{
    GENERATED_BODY()
public:
    int32 colorInputIndex = 0;
    virtual void SetupInputsOnConstruct() override;

    UPROPERTY(EditAnywhere, Category=UWKRadialPixelShader)
    float mixScalar = 0.5f;

#if WITH_EDITOR

    virtual FString NodeName() const override {
        return TEXT("WKRadialPixelShader");
    }


    int32 ValidMixScalar(FMaterialCompiler *Compiler, float scalar);
    int32 InputColor(FMaterialCompiler *Compiler);

    virtual int32 CompilePixelShader(FMaterialCompiler *Compiler) override;

#endif


};